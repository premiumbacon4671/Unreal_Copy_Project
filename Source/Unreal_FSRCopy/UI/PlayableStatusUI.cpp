// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayableStatusUI.h"
#include "PlayableCharacter/PlayableBaseCharacter.h"
#include "PlayableCharacter/Miyamoto_Iori/Miyamoto_Iori.h"
#include "PlayableCharacter/Miyamoto_Iori/ActorComponent/EarthStanceActorComponent.h"
#include "ActorComponent/StateComponent/Miyamoto_IoriStateComponent.h"
#include "ActorComponent/ResonanceComponent/ResonanceComponent.h"
#include "UI/EarthStanceShieldUI.h"
#include "UI/ServantChargeBarUI.h"
#include "PlayerState/FatePlayerState.h"
#include "PlayableCharacter/Servant/ServantBaseCharacter.h"
#include "Controller/MiyamotoIoriController/MiyamotoIoriController.h"

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
	RefreshServantUI();
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
	HandleUpdateHp(PlayerStatus, PlayerStatus->GetHPPercent());
	HandleUpdateHiken(PlayerStatus->GetHikenPercent());

	PlayerStatus->OnUpdateHpSignature.RemoveAll(this);
	PlayerStatus->OnCalculateHikenGauge.RemoveAll(this);

	PlayerStatus->OnUpdateHpSignature.AddDynamic(this, &UPlayableStatusUI::HandleUpdateHp);

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

		
		ResonanceComp->OnServantGaugeChanged.RemoveAll(this);
		ResonanceComp->OnServantGaugeChanged.AddDynamic(this, &UPlayableStatusUI::HandleUpdateServantChargeBar);
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

void UPlayableStatusUI::SetServantChargeBarPercent(int Index, float Percent)
{
	AFatePlayerState* FatePlayerState = Cast<AFatePlayerState>(GetOwningPlayerState());
	if (!FatePlayerState || !FatePlayerState->ResonanceComponent) return;

	int32 FocusedIndex = FatePlayerState->ResonanceComponent->GetFocusedServantIndex();

	if (Index == FocusedIndex)
	{
		if (ServantChargeBar1)
			ServantChargeBar1->UpdateServantChargeBar(Percent);
	}
	else
	{
		if (ServantChargeBar2)
			ServantChargeBar2->UpdateServantChargeBar(Percent);
	}
}

void UPlayableStatusUI::InitServantGaugeBar(AServantBaseCharacter* Servant, int Index)
{
	if (ServantChargeBar1 && Index == 0)
	{
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC)
		{
			AFatePlayerState* PS = PC->GetPlayerState<AFatePlayerState>();
			if (PS && PS->ResonanceComponent)
			{
				UResonanceComponent* RC = PS->ResonanceComponent;

				if (Index == 0 && ServantChargeBar1)
				{
					ServantChargeBar1->Init(Servant->GetServantIcon(), RC->GetSwapGaugePercent(Servant->GetServantName()));
				}
				else if (Index == 1 && ServantChargeBar2)
				{
					ServantChargeBar2->Init(Servant->GetServantIcon(), RC->GetSwapGaugePercent(Servant->GetServantName()));
				}
			}

		}
	}
}

void UPlayableStatusUI::RefreshServantUI()
{
	AMiyamotoIoriController* IoriController = Cast<AMiyamotoIoriController>(GetOwningPlayer());
	if (!IoriController) return;

	AFatePlayerState* FatePlayerState = Cast<AFatePlayerState>(GetOwningPlayerState());
	if (!FatePlayerState || !FatePlayerState->ResonanceComponent) return;

	UResonanceComponent* ResonanceComp = FatePlayerState->ResonanceComponent;
	const TArray<FName>& ActiveParty = ResonanceComp->GetActivePartyServants();

	int32 FocusedIndex = ResonanceComp->GetFocusedServantIndex();
	int32 OtherIndex = (FocusedIndex == 0) ? 1 : 0;
	
	if (ServantChargeBar1 && ActiveParty.IsValidIndex(FocusedIndex) && !ActiveParty[FocusedIndex].IsNone())
	{
		FName FocusedName = ActiveParty[FocusedIndex];
		AServantBaseCharacter* FocusedServant = IoriController->GetPartyServantCharacterByName(FocusedName);
		if (FocusedServant)
		{
			ServantChargeBar1->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			ServantChargeBar1->Init(FocusedServant->GetServantIcon(), ResonanceComp->GetSwapGaugePercent(FocusedName));
		}
	}
	else if (ServantChargeBar1)
	{
		ServantChargeBar1->SetVisibility(ESlateVisibility::Hidden);
	}

	if (ServantChargeBar2 && ActiveParty.IsValidIndex(OtherIndex) && !ActiveParty[OtherIndex].IsNone())
	{
		FName OtherName = ActiveParty[OtherIndex];
		AServantBaseCharacter* OtherServant = IoriController->GetPartyServantCharacterByName(OtherName);
		if (OtherServant)
		{
			ServantChargeBar2->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			ServantChargeBar2->Init(OtherServant->GetServantIcon(), ResonanceComp->GetSwapGaugePercent(OtherName));
		}
	}
	else if (ServantChargeBar2)
	{
		ServantChargeBar2->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UPlayableStatusUI::HandleUpdateHp(UBaseStateComponent* SenderComponent, float Percent)
{
	if (SenderComponent == CurrentStateComponent)
	{
		SetHPBarPercent(Percent);
	}
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

void UPlayableStatusUI::HandleUpdateServantChargeBar(int Index, float Percent)
{
	SetServantChargeBarPercent(Index, Percent);
}

void UPlayableStatusUI::SwitchTargetStatusComponent(UPlayableStateComponent* NewStatusComponent)
{
	if(CurrentStateComponent)
	{
		CurrentStateComponent->OnUpdateHpSignature.RemoveDynamic(this, &UPlayableStatusUI::HandleUpdateHp);
		CurrentStateComponent->OnCalculateHikenGauge.RemoveDynamic(this, &UPlayableStatusUI::HandleUpdateHiken);
	}

	CurrentStateComponent = NewStatusComponent;
	if (CurrentStateComponent)
	{
		CurrentStateComponent->OnUpdateHpSignature.AddUniqueDynamic(this, &UPlayableStatusUI::HandleUpdateHp);
		CurrentStateComponent->OnCalculateHikenGauge.AddUniqueDynamic(this, &UPlayableStatusUI::HandleUpdateHiken);
		HandleUpdateHp(CurrentStateComponent, CurrentStateComponent->GetHPPercent());
		HandleUpdateHiken(CurrentStateComponent->GetHikenPercent());
	}
}
