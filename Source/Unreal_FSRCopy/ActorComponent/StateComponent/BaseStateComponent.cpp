// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/StateComponent/BaseStateComponent.h"

// Sets default values for this component's properties
UBaseStateComponent::UBaseStateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBaseStateComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBaseStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBaseStateComponent::RecoverHP(float RecoverAmount)
{
	if (RecoverAmount <= 0.0)
		return;
	GetStat().CurHP = FMath::Clamp(GetStat().CurHP + RecoverAmount, 0.0f, GetStat().MaxHP);

	if (OnUpdateHp.IsBound())
		OnUpdateHp.Broadcast(GetHPPercent());
}

void UBaseStateComponent::TakeDamage(int DamageAmount)
{
	if(DamageAmount <= 0 || GetStat().CurHP <= 0)
		return;

	GetStat().CurHP = FMath::Max(0, (GetStat().CurHP - DamageAmount));

	if(OnUpdateHp.IsBound())
		OnUpdateHp.Broadcast(GetHPPercent());
}

void UBaseStateComponent::InitState(const FBaseStat& InBaseStat)
{
	GetStat() = InBaseStat; 
}

