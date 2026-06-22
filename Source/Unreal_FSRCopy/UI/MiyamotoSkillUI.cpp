// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MiyamotoSkillUI.h"

#include "UI/MiyamotoSkillButtonUI.h"
#include "PlayerState/FatePlayerState.h"
#include "ActorComponent/InventoryComponent/InventoryComponent.h"
#include "ActorComponent/SkillActionComponent/SkillActionComponent.h"
#include "DataAsset/PrimaryDataAsset/SkillDataAsset/SkillDataAsset.h"
#include "PlayableCharacter/Miyamoto_Iori/Miyamoto_Iori.h"

#include "Components/TextBlock.h"


void UMiyamotoSkillUI::NativeConstruct()
{
	Super::NativeConstruct();
	// 초기 상태 설정 (예: 모든 스킬이 사용 불가능한 상태로 시작)
	
}

void UMiyamotoSkillUI::UpdateAllSkillsAvailability(int32 CurrentGemCount)
{
	if (GemCountText)
	{
		GemCountText->SetText(FText::AsNumber(CurrentGemCount));
	}
	if (TopSkillButton)
	{
		TopSkillButton->SetSkillAvailableState(CurrentGemCount >= TopSkillButton->GetSkillCost());
	}
	if (LeftSkillButton)
	{
		LeftSkillButton->SetSkillAvailableState(CurrentGemCount >= LeftSkillButton->GetSkillCost());
	}
	if (RightSkillButton)
	{
		RightSkillButton->SetSkillAvailableState(CurrentGemCount >= RightSkillButton->GetSkillCost());
	}
	if (BottomSkillButton)
	{
		BottomSkillButton->SetSkillAvailableState(CurrentGemCount >= BottomSkillButton->GetSkillCost());
	}
}

void UMiyamotoSkillUI::Init(AMiyamoto_Iori* MiyamotoIori)
{
	if (MiyamotoIori == nullptr)
		return;
	//최적화 수정 예정
	USkillDataAsset* Data = MiyamotoIori->GetSkillActionComponent()->GetNormalSkill(0);
	if(Data == nullptr)
		return;
	TopSkillButton->UpdateSkillInfo(nullptr, Data->SkillName.ToString(), Data->CostAmount);
	Data = MiyamotoIori->GetSkillActionComponent()->GetNormalSkill(1);
	if (Data == nullptr)
		return;
	LeftSkillButton->UpdateSkillInfo(nullptr, Data->SkillName.ToString(), Data->CostAmount);

	Data = MiyamotoIori->GetSkillActionComponent()->GetNormalSkill(2);
	if (Data == nullptr)
		return;
	RightSkillButton->UpdateSkillInfo(nullptr, Data->SkillName.ToString(), Data->CostAmount);
	Data = MiyamotoIori->GetSkillActionComponent()->GetNormalSkill(3);
	if (Data == nullptr)
		return;
	BottomSkillButton->UpdateSkillInfo(nullptr, Data->SkillName.ToString(), Data->CostAmount);
	//Dummy Data
	//BottomSkillButton->UpdateSkillInfo(nullptr, TEXT("Dummy"), 999);

	AFatePlayerState* PlayerState = Cast<AFatePlayerState>(MiyamotoIori->GetPlayerState());
	UInventoryComponent* Inventory = PlayerState->InventoryComponent;
	Inventory->OnItemUpdated.AddDynamic(this, &UMiyamotoSkillUI::UpdateGemCount);
	int32 CurrentGemCount = Inventory->GetItemQuantity(FName(TEXT("Gem")));
	UpdateAllSkillsAvailability(CurrentGemCount);
	
}

void UMiyamotoSkillUI::StartedSkillUI()
{
	TopSkillButton->SetVisibility(ESlateVisibility::Visible);
	LeftSkillButton->SetVisibility(ESlateVisibility::Visible);
	RightSkillButton->SetVisibility(ESlateVisibility::Visible);
	BottomSkillButton->SetVisibility(ESlateVisibility::Visible);
}

void UMiyamotoSkillUI::EndedSkillUI()
{
	TopSkillButton->SetVisibility(ESlateVisibility::Hidden);
	LeftSkillButton->SetVisibility(ESlateVisibility::Hidden);
	RightSkillButton->SetVisibility(ESlateVisibility::Hidden);
	BottomSkillButton->SetVisibility(ESlateVisibility::Hidden);
}

void UMiyamotoSkillUI::UpdateGemCount(FName ItemID, int32 CurrentGemCount)
{
	if(ItemID == FName("Gem"))
	{
		UpdateAllSkillsAvailability(CurrentGemCount);
	}
}
