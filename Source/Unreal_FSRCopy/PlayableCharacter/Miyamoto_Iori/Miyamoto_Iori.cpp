// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter/Miyamoto_Iori/Miyamoto_Iori.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayableCharacter/Miyamoto_Iori/ActorComponent/EarthStanceActorComponent.h"

AMiyamoto_Iori::AMiyamoto_Iori()
{
	PrimaryActorTick.bCanEverTick = true;
	CurSwordStance = SWORDSTANCE::EARTH;

#pragma region CreateComponent
	FirstWeaponCoverComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstWeaponCover"));
	FirstWeaponCoverComponent->SetupAttachment(BodyComponent, FName(TEXT("FirstWeapon")));

	SwordStanceComponents.SetNum(static_cast<int32>(SWORDSTANCE::SWORDSTANCE_MAX));
	SwordStanceComponents[SWORDSTANCE::EARTH] = CreateDefaultSubobject<UEarthStanceActorComponent>(TEXT("EarthStance"));
#pragma endregion
	CurSwordStanceComponent = SwordStanceComponents[CurSwordStance];
}

void AMiyamoto_Iori::BeginPlay()
{
	Super::BeginPlay();
}

void AMiyamoto_Iori::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMiyamoto_Iori::PlayEquipWeaponMontage()
{
	if (GetMovementComponent()->IsFalling() == true ||
		nullptr == EquipMontage ||
		BodyComponent->GetAnimInstance()->Montage_IsPlaying(EquipMontage) == true)
		return;
	FName SectionName;

	switch (CurSwordStance)
	{
	case EARTH:
		SectionName = isCombatMode ? FName(TEXT("OneHandSwordEquip")) : FName(TEXT("OneHandSwordUnEquip"));
		break;
	case FIRE:
		break;
	}

	if (!SectionName.IsNone())
		PlayMontageFullBody(EquipMontage, SectionName);
}
