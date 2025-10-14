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

void UBaseStateComponent::TakeDamage(int DamageAmount)
{
	if(DamageAmount <= 0 || HP <= 0)
		return;

	const float Rand = FMath::FRandRange(0.8f, 1.2f);
	const int32 FinalDamage = FMath::Max(1, FMath::FloorToInt((DamageAmount - DefencePower) * Rand));
	HP = FMath::Max(0, HP - FinalDamage);

	if(OnTakeDamage.IsBound())
		OnTakeDamage.Execute(GetHPPercent());
}

