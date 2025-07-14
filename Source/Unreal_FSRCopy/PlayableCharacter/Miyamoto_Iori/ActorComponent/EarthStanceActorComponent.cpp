// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter/Miyamoto_Iori/ActorComponent/EarthStanceActorComponent.h"

UEarthStanceActorComponent::UEarthStanceActorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UAnimSequence> IdleSequenceFinder(
		TEXT("/Script/Engine.AnimSequence'/Game/Blueprint/PlayableCharacter/MiyamotoIori/Animation/OneHandSwordAttack/anim_idle.anim_idle'"));
	if (IdleSequenceFinder.Succeeded())
		CombatIdleSequence = IdleSequenceFinder.Object;
}

void UEarthStanceActorComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UEarthStanceActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
