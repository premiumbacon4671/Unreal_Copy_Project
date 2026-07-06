// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayableStatusUI.h"
#include "PlayableCharacter/PlayableBaseCharacter.h"
#include "PlayableCharacter/Miyamoto_Iori/Miyamoto_Iori.h"
#include "PlayableCharacter/Miyamoto_Iori/ActorComponent/EarthStanceActorComponent.h"
#include "ActorComponent/StateComponent/Miyamoto_IoriStateComponent.h"
#include "ActorComponent/ResonanceComponent/ResonanceComponent.h"
#include "UI/EarthStanceShieldUI.h"
#include "PlayerState/FatePlayerState.h"


#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "GameFramework/Pawn.h"

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
	/*if(APlayableBaseCharacter* PlayerCharacter = Cast<APlayableBaseCharacter>(GetOwningPlayerPawn()))
	{
		if(UPlayableStateComponent* PlayerStatus = PlayerCharacter->GetStatusComponent())
		{
			PlayerStatus->OnInitializedStat.AddDynamic(this, &UPlayableStatusUI::Init);
		}
	}*/
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
	CurrentStateComponent = PlayerStatus;
	HandleUpdateHp(PlayerStatus->GetHPPercent());
	HandleUpdateHiken(PlayerStatus->GetHikenPercent());

	PlayerStatus->OnUpdateHp.RemoveAll(this);
	PlayerStatus->OnCalculateHikenGauge.RemoveAll(this);

	PlayerStatus->OnUpdateHp.AddDynamic(this, &UPlayableStatusUI::HandleUpdateHp);

	PlayerStatus->OnCalculateHikenGauge.AddDynamic(this, &UPlayableStatusUI::HandleUpdateHiken);

	AFatePlayerState* PlayerState = Character->GetPlayerState<AFatePlayerState>();
	UResonanceComponent* ResonanceComp = PlayerState->ResonanceComponent;
	if(PlayerState && ResonanceComp && ResonanceComp->GetServantActive())
	{
		HandleUpdateLinkSkillGauge(ResonanceComp->GetLinkSkillGaugePercentage());
		int LinkBallCount = ResonanceComp->GetLinkBall();
		HandleUpdateLinkSkillBall(LinkBallCount);

		ResonanceComp->OnCalculateLinkSkillGauge.RemoveAll(this);
		ResonanceComp->OnCalculateLinkBall.RemoveAll(this);

		ResonanceComp->OnCalculateLinkSkillGauge.AddDynamic(this, &UPlayableStatusUI::HandleUpdateLinkSkillGauge);

		ResonanceComp->OnCalculateLinkBall.AddDynamic(this, &UPlayableStatusUI::HandleUpdateLinkSkillBall);
	}
	AMiyamoto_Iori* Miyamoto = Cast<AMiyamoto_Iori>(Character);
	if (Miyamoto)
	{
		UEarthStanceActorComponent* EarthStanceComp = Miyamoto->GetEarthStance();
		if (EarthStanceComp)
			EarthStanceShieldBar->InitShieldUI(EarthStanceComp);
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

void UPlayableStatusUI::HandleUpdateHp(float Percent)
{
	SetHPBarPercent(Percent);
}

void UPlayableStatusUI::HandleUpdateHiken(float Percent)
{
	SetHikenBarPercent(Percent);
}

void UPlayableStatusUI::HandleUpdateLinkSkillGauge(float Percent)
{
	SetLinkBarPercent(Percent);
}

void UPlayableStatusUI::HandleUpdateLinkSkillBall(int Count)
{
	for (int i = 0; i < LinkBalls.Num(); ++i)
	{
		SetLinkBallVisibility(i, i < Count ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UPlayableStatusUI::SwitchTargetStatusComponent(UPlayableStateComponent* NewStatusComponent)
{
	if(CurrentStateComponent)
	{
		CurrentStateComponent->OnUpdateHp.RemoveDynamic(this, &UPlayableStatusUI::HandleUpdateHp);
		CurrentStateComponent->OnCalculateHikenGauge.RemoveDynamic(this, &UPlayableStatusUI::HandleUpdateHiken);
	}

	CurrentStateComponent = NewStatusComponent;
	if (CurrentStateComponent)
	{
		CurrentStateComponent->OnUpdateHp.AddUniqueDynamic(this, &UPlayableStatusUI::HandleUpdateHp);
		CurrentStateComponent->OnCalculateHikenGauge.AddUniqueDynamic(this, &UPlayableStatusUI::HandleUpdateHiken);
		HandleUpdateHp(CurrentStateComponent->GetHPPercent());
		HandleUpdateHiken(CurrentStateComponent->GetHikenPercent());
	}
}
