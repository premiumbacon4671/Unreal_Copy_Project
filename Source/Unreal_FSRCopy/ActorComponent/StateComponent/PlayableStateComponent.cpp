// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/StateComponent/PlayableStateComponent.h"

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
