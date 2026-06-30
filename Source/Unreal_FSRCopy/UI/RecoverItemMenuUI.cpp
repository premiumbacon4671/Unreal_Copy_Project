// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RecoverItemMenuUI.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

#include "Controller/MiyamotoIoriController/MiyamotoIoriController.h"
#include "DataAsset/PrimaryDataAsset/ItemDataAsset/ItemDataAsset.h"
#include "ActorComponent/InventoryComponent/InventoryComponent.h"
#include "PlayerState/FatePlayerState.h"

URecoverItemMenuUI::URecoverItemMenuUI(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<URecoverItemButton> ButtonClassFinder(
		TEXT("/Game/Blueprint/PlayableCharacter/UI/BP_RecoverItemButton.BP_RecoverItemButton_C"));
	if (ButtonClassFinder.Succeeded())
		ItemButtonClass = ButtonClassFinder.Class;
}

void URecoverItemMenuUI::RefreshItemList(const TArray<FItemStack>& InItemList)
{
	if (!ItemMenu || !ItemButtonClass)
		return;
	ItemMenu->ClearChildren();
	CreatedButtons.Empty();

	for (const FItemStack& Data : InItemList)
	{
		URecoverItemButton* NewButton = CreateWidget<URecoverItemButton>(this, ItemButtonClass);
		if (NewButton)
		{
			NewButton->InitButtonData(Data, this);
			ItemMenu->AddChild(NewButton);
			CreatedButtons.Add(NewButton);
		}
	}
	if (CreatedButtons.Num() > 0)
		CurrentSelectedIndex = FMath::Clamp(CurrentSelectedIndex, 0, CreatedButtons.Num() - 1);
	else
		CurrentSelectedIndex = 0;
}

void URecoverItemMenuUI::UpdateDescription(const FText& NewDescription)
{
	if (ItemDescription)
	{
		ItemDescription->SetText(NewDescription);
	}
}

void URecoverItemMenuUI::OnInterfaceOpen()
{
	APlayerController* PC = GetOwningPlayer();
	if (!PC)
		return;
	AMiyamotoIoriController* MIPC = Cast<AMiyamotoIoriController>(PC);
	if (!MIPC)
		return;
	CurPlayableCharacter = MIPC->GetCurPlayableCharacter();
	if (CreatedButtons.Num() > 0)
	{
		CurrentSelectedIndex = FMath::Clamp(CurrentSelectedIndex, 0, CreatedButtons.Num() - 1);
		URecoverItemButton* SelectedButton = CreatedButtons[CurrentSelectedIndex];
		SelectedButton->SetHighlightState(true);
		ItemMenu->ScrollWidgetIntoView(SelectedButton, true, EDescendantScrollDestination::Center);
	}
}

void URecoverItemMenuUI::OnInterfaceClose()
{
	for (URecoverItemButton* Btn : CreatedButtons)
	{
		Btn->SetHighlightState(false);
	}
	SetVisibility(ESlateVisibility::Hidden);
}

void URecoverItemMenuUI::OnInterfaceMove(FIntPoint MoveDirection)
{
	if (CreatedButtons.Num() == 0)
		return;
	CreatedButtons[CurrentSelectedIndex]->SetHighlightState(false);
	if (MoveDirection.X > 0)
		CurrentSelectedIndex--;
	else if (MoveDirection.X < 0)
		CurrentSelectedIndex++;
	CurrentSelectedIndex = FMath::Clamp(CurrentSelectedIndex, 0, CreatedButtons.Num() - 1);

	URecoverItemButton* SelectedButton = CreatedButtons[CurrentSelectedIndex];
	SelectedButton->SetHighlightState(true);
	ItemMenu->ScrollWidgetIntoView(SelectedButton, true, EDescendantScrollDestination::Center);
}

void URecoverItemMenuUI::OnInterfaceConfirm()
{
	if (CreatedButtons.Num() == 0 || CurPlayableCharacter == nullptr)
		return;
	URecoverItemButton* SelectedButton = CreatedButtons[CurrentSelectedIndex];
	if (!SelectedButton)
		return;
	FName TargetItemID = SelectedButton->GetItemID();
	if (TargetItemID.IsNone())
		return;
	UInventoryComponent* InvComp = Cast<AFatePlayerState>(GetOwningPlayerState())->InventoryComponent;
	if (!InvComp)
		return;
	if (InvComp->UseRecoverItem(TargetItemID))
	{
		int32 NewCount = InvComp->GetItemQuantity(TargetItemID);
		SelectedButton->UpdateCounterDisplay(NewCount);
	}
}

void URecoverItemMenuUI::OnInterfaceCancel()
{
	OnInterfaceClose();
}
