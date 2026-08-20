// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MiyamotoSkillButtonUI.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"

void UMiyamotoSkillButtonUI::NativePreConstruct()
{
	Super::NativePreConstruct();
	// 초기 상태 설정 (예: 스킬이 사용 불가능한 상태로 시작)
	SetSkillAvailableState(false);
	if (!SkillIcon || !SkillName || !SkillCostText)
		return;
	UCanvasPanelSlot* NameSlot = Cast<UCanvasPanelSlot>(SkillName->Slot);
	UCanvasPanelSlot* CostSlot = Cast<UCanvasPanelSlot>(SkillCostText->Slot);

	if(!NameSlot || !CostSlot)
		return;

	SkillIcon->SetRenderTransformAngle(0.f);

	switch (ButtonDirection)
	{
	case ESkillButtonDirection::Top:
	case ESkillButtonDirection::Left:
		NameSlot->SetAlignment(FVector2D(1.15f, 1.0f));
		SkillName->SetJustification(ETextJustify::Right);
		CostSlot->SetAlignment(FVector2D(1.15f, 0.0f));
		SkillCostText->SetJustification(ETextJustify::Right);
		break;
	case ESkillButtonDirection::Bottom:
	case ESkillButtonDirection::Right:
		NameSlot->SetAlignment(FVector2D(-0.15f, 1.0f));
		SkillName->SetJustification(ETextJustify::Left);
		CostSlot->SetAlignment(FVector2D(-0.15f, 0.0f));
		SkillCostText->SetJustification(ETextJustify::Left);
		break;
	}
}

void UMiyamotoSkillButtonUI::UpdateSkillInfo(UTexture2D* IconTexture, const FString& UpdateSkillName, int32 Cost)
{
	if(SkillIcon && IconTexture)
	{
		SkillIcon->SetBrushFromTexture(IconTexture);
	}
	if (SkillName)
	{
		SkillName->SetText(FText::FromString(UpdateSkillName));
	}
	if (SkillCostText)
	{
		SkillCostText->SetText(FText::AsNumber(Cost));
		SkillCost = Cost;
	}
}

void UMiyamotoSkillButtonUI::SetSkillAvailableState(bool bIsAvailable)
{
	if (bIsAvailable)
	{
		SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
	}
	else
	{
		SetColorAndOpacity(FLinearColor(0.3f, 0.3f, 0.3f, 1.0f));
	}
}
