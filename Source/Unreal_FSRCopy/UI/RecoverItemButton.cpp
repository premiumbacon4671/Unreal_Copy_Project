// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RecoverItemButton.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"
#include "Engine/Texture2D.h"
#include "Kismet/GameplayStatics.h"

#include "UI/RecoverItemMenuUI.h"
#include "DataAsset/PrimaryDataAsset/ItemDataAsset/ItemDataAsset.h"

void URecoverItemButton::NativeConstruct()
{
	Super::NativeConstruct();
}

void URecoverItemButton::InitButtonData(const FItemStack& InData, URecoverItemMenuUI* InOwnerMenu)
{
	OwnerMenu = InOwnerMenu;
	ItemDescription = InData.ItemData->ItemDescription;
	if (ItemName)
		ItemName->SetText(InData.ItemData->ItemName);
	if (ItemCount)
		ItemCount->SetText(FText::FromString(FString::Printf(TEXT("%d"), InData.Quantity)));
	if (Icon && InData.ItemData->ItemIcon)
		Icon->SetBrushFromTexture(InData.ItemData->ItemIcon);
	ItemID = InData.ItemData->ItemID;
	SetHighlightState(false);
}

void URecoverItemButton::SetHighlightState(bool bIsHighlighted)
{
	if (nullptr == StyleSwitcher)
		return;
	int TargetIndex = bIsHighlighted ? 1 : 0;
	StyleSwitcher->SetActiveWidgetIndex(TargetIndex);
	FLinearColor TargetColor = bIsHighlighted ? TextColorSelected : TextColorNormal;

	if (ItemName)
		ItemName->SetColorAndOpacity(TargetColor);
	if (ItemCountX)
		ItemCountX->SetColorAndOpacity(TargetColor);
	if (ItemCount)
		ItemCount->SetColorAndOpacity(TargetColor);
	if (bIsHighlighted && OwnerMenu)
	{
		OwnerMenu->UpdateDescription(ItemDescription);
	}
}

void URecoverItemButton::UpdateCounterDisplay(int32 NewCount)
{
	if (ItemCount)
		ItemCount->SetText(FText::FromString(FString::Printf(TEXT("%d"), NewCount)));

}
