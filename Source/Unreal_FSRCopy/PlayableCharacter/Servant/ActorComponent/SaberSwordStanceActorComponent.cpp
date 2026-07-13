// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter/Servant/ActorComponent/SaberSwordStanceActorComponent.h"

USaberSwordStanceActorComponent::USaberSwordStanceActorComponent()
{
	WeaponVFXTarget = EWeaponVFXTarget::RightWeapon;
}

void USaberSwordStanceActorComponent::BeginPlay()
{
	Super::BeginPlay();
}

