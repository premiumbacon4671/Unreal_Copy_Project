// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/ResonanceComponent/ResonanceComponent.h"

// Sets default values for this component's properties
UResonanceComponent::UResonanceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UResonanceComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UResonanceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UResonanceComponent::RecoverLinkSkillGauge(float Amount)
{
	LinkSkillGauge += Amount;
	while (LinkSkillGauge >= MaxLinkSkillGauge && LinkBall < MaxLinkBall)
	{
		LinkBall++;
		LinkSkillGauge -= MaxLinkSkillGauge;
		if (OnCalculateLinkBall.IsBound())
			OnCalculateLinkBall.Execute(LinkBall);
	}

	if (LinkSkillGauge >= MaxLinkSkillGauge)
		LinkSkillGauge = FMath::Clamp(LinkSkillGauge, 0.0f, MaxLinkSkillGauge);

	if (OnCalculateLinkSkillGauge.IsBound())
		OnCalculateLinkSkillGauge.Execute(GetLinkSkillGaugePercentage());
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

