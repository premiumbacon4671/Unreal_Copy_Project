// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/ResonanceComponent/ResonanceComponent.h"
#include "Controller/MiyamotoIoriController/MiyamotoIoriController.h"
#include "PlayableCharacter/Servant/ServantBaseCharacter.h"

#include "GameFramework/PlayerState.h"

// Sets default values for this component's properties
UResonanceComponent::UResonanceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	ActivePartyServants.Init(NAME_None, 2);
	FocusedServantIndex = 0;
	// ...
}


// Called when the game starts
void UResonanceComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (APlayerState* OwnerPlayerState = Cast<APlayerState>(GetOwner()))
	{
		if (AMiyamotoIoriController* IoriController = Cast<AMiyamotoIoriController>(OwnerPlayerState->GetPlayerController()))
		{
			IoriController->SetupResonanceSystem(this);
		}
	}
}


// Called every frame
void UResonanceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	//서번트 조종이 아니면 무시
	if (CurrentActiveServantName.IsNone())
		return;

	if (ServantSwapGaugeMap.Contains(CurrentActiveServantName) && ActivePartyServants.Contains(CurrentActiveServantName))
	{
		float CurrentGauge = ServantSwapGaugeMap[CurrentActiveServantName];
		//게이지가 0이 되었어도 공격, 스킬을 사용 중이면
		//게이지의 감소가 없어야 하기 때문에 0.0보다 끌때만 게이지 감소
		if (CurrentGauge > 0.0f)
		{
			float NewGauge = CurrentGauge - (SwapSageDecreaseRate * DeltaTime);
			ServantSwapGaugeMap[CurrentActiveServantName] = FMath::Max(NewGauge, 0.0f);
			if (OnServantGaugeChanged.IsBound())
			{
				int32 Index = ActivePartyServants.IndexOfByKey(CurrentActiveServantName);
				OnServantGaugeChanged.Broadcast(Index, GetSwapGaugePercent(CurrentActiveServantName));
			}

			if (ServantSwapGaugeMap[CurrentActiveServantName] <= 0.0f)
			{
				CurrentActiveServantName = NAME_None;

				if (OnServantGaugeDepleted.IsBound())
				{
					OnServantGaugeDepleted.Broadcast();
				}
			}
		}
	}
}

void UResonanceComponent::RecoverLinkSkillGauge(float Amount)
{
	LinkSkillGauge += Amount;
	while (LinkSkillGauge >= MaxLinkSkillGauge && LinkBall < MaxLinkBall)
	{
		LinkBall++;
		LinkSkillGauge -= MaxLinkSkillGauge;
		if (OnCalculateLinkBall.IsBound())
			OnCalculateLinkBall.Broadcast(LinkBall);
	}

	if (LinkSkillGauge >= MaxLinkSkillGauge)
		LinkSkillGauge = FMath::Clamp(LinkSkillGauge, 0.0f, MaxLinkSkillGauge);

	if (OnCalculateLinkSkillGauge.IsBound())
		OnCalculateLinkSkillGauge.Broadcast(GetLinkSkillGaugePercentage());
}

void UResonanceComponent::CalculateLinkSkillGauge(int AmountDamage, float TargetMaxHP)
{
	if(TargetMaxHP <= 0.0f)
		return;

	float GaugeIncrease = (AmountDamage / TargetMaxHP) * LinkSkillGaugeGainMultiplier;
	RecoverLinkSkillGauge(GaugeIncrease);
}

bool UResonanceComponent::ConsumeLinkBall(int32 Count)
{
	LinkBall -= Count;
	return false;
}

void UResonanceComponent::InitResonance(float fMaxLinkSkillGauge, float fLinkSkillGauge, int32 iMaxLinkBall, int32 iLinkBall)
{
	MaxLinkSkillGauge = fMaxLinkSkillGauge;
	LinkSkillGauge = fLinkSkillGauge;
	MaxLinkBall = iMaxLinkBall;
	LinkBall = iLinkBall;
}

void UResonanceComponent::AddSaberGauge(float Amount)
{
	CurrentSaberGauge = FMath::Clamp(CurrentSaberGauge + Amount, 0.0f, MaxSaberGauge);
	//OnSaberGaugeChanged.Broadcast(GetSaberGaugePercent());
}

void UResonanceComponent::ConsumeSaberGauge(float Amount)
{
	CurrentSaberGauge = FMath::Clamp(CurrentSaberGauge - Amount, 0.0f, MaxSaberGauge);
	//OnSaberGaugeChanged.Broadcast(GetSaberGaugePercent());
}

void UResonanceComponent::RegisterServantToParty(FName ServantName)
{
	if(ServantName.IsNone())
		return;
	if(!ServantSwapGaugeMap.Contains(ServantName))
	{
		ServantSwapGaugeMap.Add(ServantName, 60.0f);
	}
}

void UResonanceComponent::SetSecondaryPartyServant(FName ServantName)
{
	if (ServantName.IsNone())
	{
		ActivePartyServants[1] = NAME_None;
		return;
	}
	if(ServantSwapGaugeMap.Contains(ServantName))
	{
		if (ServantName == TEXT("Saber"))
		{
			ActivePartyServants[0] = ServantName;
		}
		else
		{
			ActivePartyServants[1] = ServantName;
		}
	}
	else
	{
		ActivePartyServants[1] = NAME_None;
		UE_LOG(LogTemp, Warning, TEXT("Servant %s is not registered in the party."), *ServantName.ToString());
	}
}

void UResonanceComponent::CalculateServnatGauge(int AmountDamage, float TargetMaxHP)
{
	if (TargetMaxHP <= 0.0f)
		return;

	float GaugeIncrease = (AmountDamage / TargetMaxHP) * ServantGaugeGainMultiplier;
	AddSwapGauge(GaugeIncrease);
}

void UResonanceComponent::AddSwapGauge(float Amount)
{
	if(Amount <= 0.0f)
		return;
	for (const FName& ServantName : ActivePartyServants)
	{
		if(ServantName.IsNone())
			continue;
		if (ServantSwapGaugeMap.Contains(ServantName))
		{
			float CurrentGauge = ServantSwapGaugeMap[ServantName];

			float NewGauge = FMath::Clamp(CurrentGauge + Amount, 0.0f, MaxServantSwapGauge);
			ServantSwapGaugeMap[ServantName] = NewGauge;
			if (OnServantGaugeChanged.IsBound())
			{
				int32 Index = ActivePartyServants.IndexOfByKey(ServantName);
				OnServantGaugeChanged.Broadcast(Index, GetSwapGaugePercent(ServantName));
			}
		}
	}
}

void UResonanceComponent::SetActiveServant(FName ServantName)
{
	CurrentActiveServantName = ServantName;
}

bool UResonanceComponent::IsSwapAvailable(FName ServantName) const
{
	if (ServantSwapGaugeMap.Contains(ServantName))
	{
		return ServantSwapGaugeMap[ServantName] >= MaxServantSwapGauge;
	}
	return false;
}

float UResonanceComponent::GetSwapGaugePercent(FName ServantName) const
{
	if (ServantSwapGaugeMap.Contains(ServantName) && MaxServantSwapGauge > 0.0f)
	{
		return ServantSwapGaugeMap[ServantName] / MaxServantSwapGauge;
	}
	return 0.0f;
}

int UResonanceComponent::GetActivePartyServantIndex(FName ServantName) const
{
	if (ActivePartyServants.Contains(ServantName))
	{
		return ActivePartyServants.IndexOfByKey(ServantName);
	}
	return -1;
}

FName UResonanceComponent::GetFocusedServantName() const
{
	if (ActivePartyServants.IsValidIndex(FocusedServantIndex))
	{
		return ActivePartyServants[FocusedServantIndex];
	}
	return NAME_None;
}

void UResonanceComponent::CycleFocusedServant(int32 Direction)
{
	int32 ValidCount = 0;
	for (const FName& Name : ActivePartyServants)
	{
		if (!Name.IsNone())
			ValidCount++;
	}

	if (ValidCount <= 1)
		return;
	do
	{
		FocusedServantIndex += Direction;

		if (FocusedServantIndex >= ActivePartyServants.Num())
		{
			FocusedServantIndex = 0;
		}
		else if (FocusedServantIndex < 0)
		{
			FocusedServantIndex = ActivePartyServants.Num() - 1;
		}
	} while (ActivePartyServants[FocusedServantIndex].IsNone());
}
