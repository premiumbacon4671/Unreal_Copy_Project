// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/StateComponent/PlayableStateComponent.h"
#include "PlayableCharacter/PlayableBaseCharacter.h"

UPlayableStateComponent::UPlayableStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayableStateComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayableStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPlayableStateComponent::InitState(const FBaseStat& InBaseStat)
{
	PlayableStat = static_cast<const FPlayableStat&>(InBaseStat);
	OnInitializedStat.Broadcast(Cast<APlayableBaseCharacter>(GetOwner()));
}

void UPlayableStateComponent::CalculateHikenGauge(float AmountDamage, float TargetMaxHP)
{
	if (TargetMaxHP <= 0.0f)
		return;

	float GaugeIncrease = (AmountDamage / TargetMaxHP) * HikenGainMultiplier;
	GetPlayableStat().Hiken += GaugeIncrease;

	//°ø¸í ±¸½½ È¹µæ

	if (GetPlayableStat().Hiken >= GetPlayableStat().MaxHiken)
		GetPlayableStat().Hiken = FMath::Clamp(GetPlayableStat().Hiken, 0.0f, GetPlayableStat().MaxHiken);

	if (OnCalculateHikenGauge.IsBound())
		OnCalculateHikenGauge.Execute(GetHikentPercent());
}
