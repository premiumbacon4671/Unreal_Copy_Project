// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/StateComponent/Miyamoto_IoriStateComponent.h"
#include "Miyamoto_IoriStateComponent.h"

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
