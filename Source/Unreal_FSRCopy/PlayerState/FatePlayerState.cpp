// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/FatePlayerState.h"
#include "ActorComponent/ResonanceComponent/ResonanceComponent.h"
#include "ActorComponent/InventoryComponent/InventoryComponent.h"

AFatePlayerState::AFatePlayerState()
{
	ResonanceComponent = CreateDefaultSubobject<UResonanceComponent>(TEXT("ResonanceComponent"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
}
