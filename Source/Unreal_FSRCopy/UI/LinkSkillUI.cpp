// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LinkSkillUI.h"

#include "UI/LinkSkillButtonUI.h"
#include "PlayableCharacter/Servant/ServantBaseCharacter.h"
#include "ActorComponent/ResonanceComponent/ResonanceComponent.h"
#include "ActorComponent/SkillActionComponent/SkillActionComponent.h"
#include "PlayerState/FatePlayerState.h"
#include "DataAsset/PrimaryDataAsset/SkillDataAsset/SkillDataAsset.h"
#include "Controller/MiyamotoIoriController/MiyamotoIoriController.h"
#include "UI/ServantChargeBarUI.h"
#include "ActorComponent/StateComponent/PlayableStateComponent.h"

#include "Components/Image.h"
#include "Components/ProgressBar.h"

void ULinkSkillUI::NativeConstruct()
{
	Super::NativeConstruct();
	LinkBalls.Add(LinkBall1);
	LinkBalls.Add(LinkBall2);
	LinkBalls.Add(LinkBall3);
	LinkBalls.Add(LinkBall4);
	LinkBalls.Add(LinkBall5);
	LinkBalls.Add(LinkBall6);
	AMiyamotoIoriController* Controller = Cast<AMiyamotoIoriController>(GetOwningPlayer());
	if (Controller)
	{
		AServantBaseCharacter* Servant = Controller->GetSaberCharacter();
		if(Servant)
		{
			Init(Servant);
		}
	}
}

void ULinkSkillUI::UpdateAllLinkSkillsAvailability(int32 CurrentLinkCount, float CurrentLinkProgress)
{
	if (LinkBalls.Num() == 6)
	{
		for (int i = 0; i < LinkBalls.Num(); i++)
		{
			if(i < CurrentLinkCount)
			{
				LinkBalls[i]->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				LinkBalls[i]->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
	if (LinkProgressBar)
	{
		LinkProgressBar->SetPercent(CurrentLinkProgress);
	}
	if (TopLinkSkillButton)
	{
		TopLinkSkillButton->SetUpdateLinkBallVisibility(CurrentLinkCount);
	}
	if (LeftLinkSkillButton)
	{
		LeftLinkSkillButton->SetUpdateLinkBallVisibility(CurrentLinkCount);
	}
	if (RightLinkSkillButton)
	{
		RightLinkSkillButton->SetUpdateLinkBallVisibility(CurrentLinkCount);
	}
	if (BottomLinkSkillButton)
	{
		BottomLinkSkillButton->SetUpdateLinkBallVisibility(CurrentLinkCount);
	}
}

void ULinkSkillUI::Init(AServantBaseCharacter* Servant)
{
	if (nullptr == Servant)
		return;
	//Dummy Data
	TopLinkSkillButton->UpdateLinkSkillInfo(nullptr, TEXT("Top Link Skill"), 3);
	LeftLinkSkillButton->UpdateLinkSkillInfo(nullptr, TEXT("Left Link Skill"), 2);
	RightLinkSkillButton->UpdateLinkSkillInfo(nullptr, TEXT("Right Link Skill"), 1);
	BottomLinkSkillButton->UpdateLinkSkillInfo(nullptr, TEXT("Battom Link Skill"), 3);

	AFatePlayerState* PlayerState = GetOwningPlayerState<AFatePlayerState>();
	if (PlayerState && PlayerState->ResonanceComponent)
	{
		UResonanceComponent* ResonanceComponent = PlayerState->ResonanceComponent;
		UpdateAllLinkSkillsAvailability(ResonanceComponent->GetLinkBall(), ResonanceComponent->GetLinkSkillGaugePercentage());
	}

	ServantChargeBar->Init(Servant->GetServantIcon(), PlayerState->ResonanceComponent->GetSwapGaugePercent(Servant->GetServantName()));
}

void ULinkSkillUI::OnInterfaceOpen()
{
	TopLinkSkillButton->SetVisibility(ESlateVisibility::Visible);
	LeftLinkSkillButton->SetVisibility(ESlateVisibility::Visible);
	RightLinkSkillButton->SetVisibility(ESlateVisibility::Visible);
	BottomLinkSkillButton->SetVisibility(ESlateVisibility::Visible);
	AFatePlayerState* PlayerState = GetOwningPlayerState<AFatePlayerState>();
	if (PlayerState && PlayerState->ResonanceComponent)
	{
		UResonanceComponent* ResonanceComponent = PlayerState->ResonanceComponent;
		UpdateAllLinkSkillsAvailability(ResonanceComponent->GetLinkBall(), ResonanceComponent->GetLinkSkillGaugePercentage());
	}
	AMiyamotoIoriController* Controller = Cast<AMiyamotoIoriController>(GetOwningPlayer());
	if (Controller)
	{
		AServantBaseCharacter* Servant = Controller->GetSaberCharacter();
		if (Servant)
		{
			ServantChargeBar->UpdateServantChargeBar(PlayerState->ResonanceComponent->GetSwapGaugePercent(Servant->GetServantName()));
		}
	}
}

void ULinkSkillUI::OnInterfaceClose()
{
	TopLinkSkillButton->SetVisibility(ESlateVisibility::Hidden);
	LeftLinkSkillButton->SetVisibility(ESlateVisibility::Hidden);
	RightLinkSkillButton->SetVisibility(ESlateVisibility::Hidden);
	BottomLinkSkillButton->SetVisibility(ESlateVisibility::Hidden);
}

void ULinkSkillUI::OnInterfaceConfirm()
{
	AMiyamotoIoriController* Controller = Cast<AMiyamotoIoriController>(GetOwningPlayer());
	if (nullptr == Controller)
		return;
	Controller->SwapWithServant();
}

void ULinkSkillUI::OnInterfaceMove(FIntPoint MoveDirection)
{
	//2기 이상의 서번트를 보유 하고 있을 때 변경 혹은 사용할 링크 스킬 변경
	//현재는 1기만 존재하므로 미구현
}
