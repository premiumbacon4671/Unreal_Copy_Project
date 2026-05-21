// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/FatePlayerState.h"
#include "ActorComponent/ResonanceComponent/ResonanceComponent.h"

AFatePlayerState::AFatePlayerState()
{
	ResonanceComponent = CreateDefaultSubobject<UResonanceComponent>(TEXT("ResonanceComponent"));
}
