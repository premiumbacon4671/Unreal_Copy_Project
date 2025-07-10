// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter/Miyamoto_Iori/Miyamoto_Iori.h"
#include "GameFramework/CharacterMovementComponent.h"

AMiyamoto_Iori::AMiyamoto_Iori()
{
	PrimaryActorTick.bCanEverTick = true;
	CurSwordStance = SwordStance::EARTH;
}

void AMiyamoto_Iori::BeginPlay()
{
	Super::BeginPlay();
}

void AMiyamoto_Iori::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
