// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter/Miyamoto_Iori/Miyamoto_Iori.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayableCharacter/Miyamoto_Iori/ActorComponent/EarthStanceActorComponent.h"

AMiyamoto_Iori::AMiyamoto_Iori()
{
	PrimaryActorTick.bCanEverTick = true;
	CurSwordStance = ESWORDSTANCE::EST_EARTH;

#pragma region CreateComponent
	FirstWeaponCoverComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstWeaponCover"));
	FirstWeaponCoverComponent->SetupAttachment(BodyComponent, FName(TEXT("FirstWeapon")));

	//SwordStanceComponents.SetNum(static_cast<int32>(ESWORDSTANCE::EST_MAX));
	//SwordStanceComponents[static_cast<int32>(CurSwordStance)] = CreateDefaultSubobject<UEarthStanceActorComponent>(TEXT("EarthStance"));

	UEarthStanceActorComponent* EarthStance = CreateDefaultSubobject<UEarthStanceActorComponent>(TEXT("EarthStanceComponent"));
	//SwordStanceComponents[static_cast<int32>(ESWORDSTANCE::EST_EARTH)] = EarthStance;
	SwordStanceComponents.Add(EarthStance);

#pragma endregion
	CurSwordStanceComponent = SwordStanceComponents[static_cast<int32>(CurSwordStance)];
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
		BodyComponent->GetAnimInstance()->Montage_IsPlaying(EquipMontage) == true ||
		BodyComponent->GetAnimInstance()->Montage_IsPlaying(CurSwordStanceComponent->GetNormalAttackMontage()) == true)
		return;
	FName SectionName;

	switch (CurSwordStance)
	{
	case ESWORDSTANCE::EST_EARTH:
		SectionName = isCombatMode ? FName(TEXT("OneHandSwordEquip")) : FName(TEXT("OneHandSwordUnEquip"));
		break;
	case ESWORDSTANCE::EST_FIRE:
		break;
	}

	if (!SectionName.IsNone())
		PlayMontageFullBody(EquipMontage, SectionName);
}
