// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LinkSkillButtonUI.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"

void ULinkSkillButtonUI::NativePreConstruct()
{
	Super::NativePreConstruct();
	// 초기 상태 설정 (예: 스킬이 사용 불가능한 상태로 시작)
	SetLinkSkillAvailableState(false);
	if(!SkillIcon || !SkillName || !LinkBall1 || !LinkBall2 || !LinkBall3 || !LinkBallFrame1 || !LinkBallFrame2 || !LinkBallFrame3)
		return;

	LinkBalls = { LinkBall1, LinkBall2, LinkBall3 };
	LinkBallFrames = { LinkBallFrame1, LinkBallFrame2, LinkBallFrame3 };

	UCanvasPanelSlot* NameSlot = Cast<UCanvasPanelSlot>(SkillName->Slot);
	TArray<UCanvasPanelSlot*> LinkBallSlots;
	TArray<UCanvasPanelSlot*> LinkBallFrameSlots;
	for (int i = 0; i < LinkBalls.Num(); i++)
	{
		LinkBallSlots.Add(Cast<UCanvasPanelSlot>(LinkBalls[i]->Slot));
		LinkBallFrameSlots.Add(Cast<UCanvasPanelSlot>(LinkBallFrames[i]->Slot));
	}
	

	SkillIcon->SetRenderTransformAngle(0.f);

	switch (ButtonDirection)
	{
	case ELinkSkillButtonDirection::Top:
	case ELinkSkillButtonDirection::Left:
	{
		NameSlot->SetAlignment(FVector2D(1.15f, 1.0f));
		SkillName->SetJustification(ETextJustify::Right);
		float LinkBallOffset = 1.1f;
		for (int i = 0; i < LinkBalls.Num(); i++)
		{
			LinkBallSlots[i]->SetAlignment(FVector2D(2.6f + (i * LinkBallOffset), -0.2f));
			LinkBallFrameSlots[i]->SetAlignment(FVector2D(2.6f + (i * LinkBallOffset), -0.2f));
		}
	}
		break;
	case ELinkSkillButtonDirection::Right:
	case ELinkSkillButtonDirection::Bottom:
	{
		NameSlot->SetAlignment(FVector2D(-0.15f, 1.0f));
		SkillName->SetJustification(ETextJustify::Left);
		float LinkBallOffset = -1.1f;
		for (int i = 0; i < LinkBalls.Num(); i++)
		{
			LinkBallSlots[i]->SetAlignment(FVector2D(-1.6f + (i * LinkBallOffset), -0.2f));
			LinkBallFrameSlots[i]->SetAlignment(FVector2D(-1.6f + (i * LinkBallOffset), -0.2f));
		}
	}
		break;
	}
}

void ULinkSkillButtonUI::UpdateLinkSkillInfo(UTexture2D* IconTexture, const FString& UpdateSkillName, int32 Cost)
{
	if (SkillIcon && IconTexture)
	{
		SkillIcon->SetBrushFromTexture(IconTexture);
	}
	if (SkillName)
	{
		SkillName->SetText(FText::FromString(UpdateSkillName));
	}
	if (LinkBallFrames.Num() > 0)
	{
		SkillCost = Cost;
		for (int i = 0; i < LinkBallFrames.Num(); i++)
		{
			if(i < Cost)
				LinkBallFrames[i]->SetVisibility(ESlateVisibility::Visible);
			else
				LinkBallFrames[i]->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void ULinkSkillButtonUI::SetUpdateLinkBallVisibility(int CurrentLinkCount)
{
	if (LinkBalls.Num() > 0)
	{
		for (int i = 0; i < LinkBalls.Num(); i++)
		{
			if (i < CurrentLinkCount && i < SkillCost)
			{
				LinkBalls[i]->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				LinkBalls[i]->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
	if(CurrentLinkCount >= SkillCost)
		{
		SetLinkSkillAvailableState(true);
	}
	else
	{
		SetLinkSkillAvailableState(false);
	}
}

void ULinkSkillButtonUI::SetLinkSkillAvailableState(bool bIsAvailable)
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
