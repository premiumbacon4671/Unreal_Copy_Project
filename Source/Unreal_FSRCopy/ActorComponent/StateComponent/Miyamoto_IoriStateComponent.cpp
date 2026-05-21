// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/StateComponent/Miyamoto_IoriStateComponent.h"
#include "PlayableCharacter/Miyamoto_Iori/Miyamoto_Iori.h"
#include "ActorComponent/ResonanceComponent/ResonanceComponent.h"
#include "PlayerState/FatePlayerState.h"

#include "GameFramework/PlayerState.h"
#include "GameFramework/Pawn.h"

UMiyamoto_IoriStateComponent::UMiyamoto_IoriStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMiyamoto_IoriStateComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMiyamoto_IoriStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UMiyamoto_IoriStateComponent::InitState(const FBaseStat& InBaseStat)
{
	Miyamoto_IoriStat = static_cast<const FMiyamoto_IoriStat&>(InBaseStat);
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if(OwnerPawn)
	{
		AFatePlayerState* PlayerState = OwnerPawn->GetPlayerState<AFatePlayerState>();
		if (PlayerState)
		{
			if(PlayerState->ResonanceComponent)
			{
				PlayerState->ResonanceComponent->InitResonance(Miyamoto_IoriStat.GetMaxLinkSkill(), Miyamoto_IoriStat.GetLinkSkill(), Miyamoto_IoriStat.GetMaxLinkSkillBall(), Miyamoto_IoriStat.GetLinkSkillBall());
			}
		}
	}
	OnInitializedStat.Broadcast(Cast<APlayableBaseCharacter>(GetOwner()));
}
