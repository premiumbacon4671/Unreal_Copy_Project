// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LinkSkillUI.h"

#include "UI/LinkSkillButtonUI.h"
#include "PlayableCharacter/Servant/ServantBaseCharacter.h"
#include "ActorComponent/ResonanceComponent/ResonanceComponent.h"
#include "ActorComponent/SkillActionComponent/SkillActionComponent.h"
#include "ActorComponent/StateComponent/PlayableStateComponent.h"
#include "PlayerState/FatePlayerState.h"
#include "DataAsset/PrimaryDataAsset/SkillDataAsset/SkillDataAsset.h"
#include "Controller/MiyamotoIoriController/MiyamotoIoriController.h"
#include "UI/ServantChargeBarUI.h"
#include "HUD/PlayerHUD.h"
#include "UI/PlayableStatusUI.h"

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
	/*AMiyamotoIoriController* Controller = Cast<AMiyamotoIoriController>(GetOwningPlayer());
	if (Controller)
	{
		AServantBaseCharacter* Servant = Controller->GetSaberCharacter();
		if(Servant)
		{
			Init(Servant);
		}
	}*/
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
	USkillDataAsset* Data = Servant->GetSkillActionComponent()->GetNormalSkill(0);
	if (Data == nullptr)
		return;
	TopLinkSkillButton->UpdateLinkSkillInfo(Data->Icon, Data->SkillName.ToString(), Data->CostAmount);

	//TopLinkSkillButton->UpdateLinkSkillInfo(nullptr, TEXT("Top Link Skill"), 3);
	LeftLinkSkillButton->UpdateLinkSkillInfo(nullptr, TEXT("Left Link Skill"), 2);
	RightLinkSkillButton->UpdateLinkSkillInfo(nullptr, TEXT("Right Link Skill"), 1);
	BottomLinkSkillButton->UpdateLinkSkillInfo(nullptr, TEXT("Battom Link Skill"), 3);

	AFatePlayerState* PlayerState = GetOwningPlayerState<AFatePlayerState>();
	if (PlayerState && PlayerState->ResonanceComponent)
	{
		UResonanceComponent* ResonanceComponent = PlayerState->ResonanceComponent;
		UpdateAllLinkSkillsAvailability(ResonanceComponent->GetLinkBall(), ResonanceComponent->GetLinkSkillGaugePercentage());
	}
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
		AServantBaseCharacter* FocusedServant = Controller->GetFocusedServantCharacter();
		RefreshPartyUI(FocusedServant);
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
	//Controller->SwapWithServant();
}

void ULinkSkillUI::OnInterfaceMove(FIntPoint MoveDirection)
{
	if (MoveDirection.X == 0)
		return;

	AFatePlayerState* PS = GetOwningPlayerState<AFatePlayerState>();
	if (PS && PS->ResonanceComponent)
	{
		int32 CycleDirection = (MoveDirection.X > 0) ? -1 : 1;

		PS->ResonanceComponent->CycleFocusedServant(CycleDirection);

		AMiyamotoIoriController* PC = Cast<AMiyamotoIoriController>(GetOwningPlayer());
		if (PC)
		{
			RefreshPartyUI(PC->GetFocusedServantCharacter());
			if (APlayerHUD* HUD = Cast<APlayerHUD>(PC->GetHUD()))
			{
				if (UPlayableStatusUI* StatusUI = HUD->GetPlayableStatusUI())
				{
					StatusUI->RefreshServantUI();
				}
			}
		}
	}
}

void ULinkSkillUI::RefreshPartyUI(AServantBaseCharacter* Seervant)
{
	AFatePlayerState* PS = Cast<AFatePlayerState>(GetOwningPlayerState());
	if (!PS || !PS->ResonanceComponent)
		return;
	UResonanceComponent* ResonanceComp = PS->ResonanceComponent;
	const TArray<FName>& ActivePartyServants = ResonanceComp->GetActivePartyServants();
	int32 FocusedIndex = ResonanceComp->GetFocusedServantIndex();
	int32 OtherIndex = (FocusedIndex == 0) ? 1 : 0;
	
	if (ServantChargeBar1)
	{
		if (ActivePartyServants.IsValidIndex(FocusedIndex) && !ActivePartyServants[FocusedIndex].IsNone())
		{
			ServantChargeBar1->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			UTexture2D* FocusedIcon = Seervant->GetServantIcon();
			float FocusedPercent = ResonanceComp->GetSwapGaugePercent(ActivePartyServants[FocusedIndex]);
			ServantChargeBar1->Init(FocusedIcon, FocusedPercent);
		}
		else
		{
			ServantChargeBar1->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	if (ServantChargeBar2)
	{
		if (ActivePartyServants.IsValidIndex(OtherIndex) && !ActivePartyServants[OtherIndex].IsNone())
		{
			FName OtherName = ActivePartyServants.IsValidIndex(OtherIndex) ? ActivePartyServants[OtherIndex] : NAME_None;
			AMiyamotoIoriController* PC = Cast<AMiyamotoIoriController>(GetOwningPlayer());
			AServantBaseCharacter* OtherServant = PC ? PC->GetPartyServantCharacterByName(OtherName) : nullptr;
			if (OtherServant)
			{
				ServantChargeBar2->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				UTexture2D* OtherIcon = OtherServant->GetServantIcon();
				float OtherPercent = ResonanceComp->GetSwapGaugePercent(OtherName);
				ServantChargeBar2->Init(OtherIcon, OtherPercent);
			}
			
		}
		else
		{
			ServantChargeBar2->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (Seervant)
	{
		USkillActionComponent* SkillComp = Seervant->GetSkillActionComponent();
		if (SkillComp)
		{
			if(FocusedIndex == 0)
			{
				USkillDataAsset* Data = Seervant->GetSkillActionComponent()->GetNormalSkill(0);
				if (Data == nullptr)
					return;
				TopLinkSkillButton->UpdateLinkSkillInfo(Data->Icon, Data->SkillName.ToString(), Data->CostAmount);

				LeftLinkSkillButton->UpdateLinkSkillInfo(nullptr, TEXT("Left Link Skill1"), 2);
				RightLinkSkillButton->UpdateLinkSkillInfo(nullptr, TEXT("Right Link Skill1"), 1);
				BottomLinkSkillButton->UpdateLinkSkillInfo(nullptr, TEXT("Battom Link Skill1"), 3);
			}
			else
			{
				TopLinkSkillButton->UpdateLinkSkillInfo(nullptr, TEXT("Top Link Skill2"), 3);
				LeftLinkSkillButton->UpdateLinkSkillInfo(nullptr, TEXT("Left Link Skill2"), 2);
				RightLinkSkillButton->UpdateLinkSkillInfo(nullptr, TEXT("Right Link Skill2"), 1);
				BottomLinkSkillButton->UpdateLinkSkillInfo(nullptr, TEXT("Battom Link Skill2"), 3);
			}
		}
	}
	UpdateAllLinkSkillsAvailability(ResonanceComp->GetLinkBall(), ResonanceComp->GetLinkSkillGaugePercentage());
}
