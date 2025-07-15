// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter/Miyamoto_Iori/ActorComponent/EarthStanceActorComponent.h"

UEarthStanceActorComponent::UEarthStanceActorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEarthStanceActorComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UEarthStanceActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
