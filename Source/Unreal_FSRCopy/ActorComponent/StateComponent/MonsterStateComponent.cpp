// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/StateComponent/MonsterStateComponent.h"

UMonsterStateComponent::UMonsterStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMonsterStateComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMonsterStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
