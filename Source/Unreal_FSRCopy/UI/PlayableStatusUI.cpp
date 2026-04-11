// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayableStatusUI.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "PlayableCharacter/PlayableBaseCharacter.h"
#include "PlayableCharacter/Miyamoto_Iori/Miyamoto_Iori.h"
#include "ActorComponent/StateComponent/Miyamoto_IoriStateComponent.h"

void UPlayableStatusUI::NativeConstruct()
{
	Super::NativeConstruct();
	LinkBalls.SetNum(6);
	LinkBalls[0] = LinkBall1;
	LinkBalls[1] = LinkBall2;
	LinkBalls[2] = LinkBall3;
	LinkBalls[3] = LinkBall4;
	LinkBalls[4] = LinkBall5;
	LinkBalls[5] = LinkBall6;
	if(APlayableBaseCharacter* PlayerCharacter = Cast<APlayableBaseCharacter>(GetOwningPlayerPawn()))
	{
		if(UPlayableStateComponent* PlayerStatus = PlayerCharacter->GetStatusComponent())
		{
			PlayerStatus->OnInitializedStat.AddUObject(this, &UPlayableStatusUI::Init);
		}
	}
}

void UPlayableStatusUI::SetLinkBallVisibility(int Index, ESlateVisibility InVisibility)
{
	if (Index < 0 || Index >= LinkBalls.Num())
		return;
	LinkBalls[Index]->SetVisibility(InVisibility);
}

void UPlayableStatusUI::Init(APlayableBaseCharacter* Character)
{
	if (Character == nullptr)
		return;
	UPlayableStateComponent* PlayerStatus = Character->GetStatusComponent();
	if (PlayerStatus == nullptr)
		return;
	SetHPBarPercent(PlayerStatus->GetHPPercent());
	SetHikenBarPercent(PlayerStatus->GetHikentPercent());
	UMiyamoto_IoriStateComponent* MiyamotoStatus = Cast<UMiyamoto_IoriStateComponent>(PlayerStatus);
	if (MiyamotoStatus == nullptr)
		return;
	SetLinkBarPercent(MiyamotoStatus->GetLinkSkillPercent());
	int LinkBallCount = MiyamotoStatus->GetLinkSkillBall();
	for (int i = 0; i < LinkBallCount; ++i)
	{
		SetLinkBallVisibility(i, ESlateVisibility::Visible);
	}

}

void UPlayableStatusUI::SetHPBarPercent(float Percent)
{
	if (HPProgressBar)
		HPProgressBar->SetPercent(Percent);
}

void UPlayableStatusUI::SetHikenBarPercent(float Percent)
{
	if (HikenProgressBar)
		HikenProgressBar->SetPercent(Percent);
}

void UPlayableStatusUI::SetLinkBarPercent(float Percent)
{
	if (LinkProgressBar)
		LinkProgressBar->SetPercent(Percent);
}
