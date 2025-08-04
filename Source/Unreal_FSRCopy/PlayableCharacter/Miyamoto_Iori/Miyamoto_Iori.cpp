// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter/Miyamoto_Iori/Miyamoto_Iori.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayableCharacter/Miyamoto_Iori/ActorComponent/EarthStanceActorComponent.h"
#include "PlayableCharacter/Miyamoto_Iori/ActorComponent/FireStanceActorComponent.h"

AMiyamoto_Iori::AMiyamoto_Iori()
{
	PrimaryActorTick.bCanEverTick = true;
	CurSwordStance = ESWORDSTANCE::EST_FIRE;

#pragma region CreateComponent
	//SwordStanceComponents[static_cast<int32>(CurSwordStance)] = CreateDefaultSubobject<UEarthStanceActorComponent>(TEXT("EarthStance"));

	SwordStanceComponents.SetNum(static_cast<int>(ESWORDSTANCE::EST_MAX));
	UEarthStanceActorComponent* EarthStance = CreateDefaultSubobject<UEarthStanceActorComponent>(TEXT("EarthStanceComponent"));
	SwordStanceComponents[0] = EarthStance;
	UFireStanceActorComponent* FireStance = CreateDefaultSubobject<UFireStanceActorComponent>(TEXT("FireStanceComponent"));
	SwordStanceComponents[2] = FireStance;

	UBaseSwordStanceActorComponent* a1Stance = CreateDefaultSubobject<UBaseSwordStanceActorComponent>(TEXT("a1StanceComponent"));
	SwordStanceComponents[1] = a1Stance;
	UBaseSwordStanceActorComponent* a2Stance = CreateDefaultSubobject<UBaseSwordStanceActorComponent>(TEXT("a2StanceComponent"));
	SwordStanceComponents[3] = a2Stance;
	UBaseSwordStanceActorComponent* a3Stance = CreateDefaultSubobject<UBaseSwordStanceActorComponent>(TEXT("a3StanceComponent"));
	SwordStanceComponents[4] = a3Stance;

	IsUnlockedSwordStance.Add(ESWORDSTANCE::EST_EARTH, true);
	IsUnlockedSwordStance.Add(ESWORDSTANCE::EST_WATER, false);
	IsUnlockedSwordStance.Add(ESWORDSTANCE::EST_FIRE, true);
	IsUnlockedSwordStance.Add(ESWORDSTANCE::EST_WIND, false);
	IsUnlockedSwordStance.Add(ESWORDSTANCE::EST_VOID, false);
#pragma endregion
	/*switch (CurSwordStance)
	{
	case ESWORDSTANCE::EST_EARTH:
		CurSwordStanceComponent = SwordStanceComponents[0];
		break;
	case ESWORDSTANCE::EST_FIRE:
		CurSwordStanceComponent = SwordStanceComponents[1];
		break;
	default:
		break;
	}*/
	//CurSwordStanceComponent = SwordStanceComponents[static_cast<int>(CurSwordStance)];
}

void AMiyamoto_Iori::BeginPlay()
{
	Super::BeginPlay();
	CurSwordStanceComponent = SwordStanceComponents[static_cast<int>(CurSwordStance)];
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
		SectionName = isCombatMode ? FName(TEXT("OneHandSwordEquip")) : FName(TEXT("OneHandSwordUnEquip"));
		break;
	}

	if (!SectionName.IsNone())
		PlayMontageFullBody(EquipMontage, SectionName);
}

void AMiyamoto_Iori::WeaponEquip()
{
	Super::WeaponEquip();
	switch (CurSwordStance)
	{
	case ESWORDSTANCE::EST_EARTH:
		break;
	case ESWORDSTANCE::EST_FIRE:
		SecondWeaponComponent->AttachToComponent(BodyComponent,
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true),
		FName(TEXT("SecondWeaponHand")));
		break;
	default:
		break;
	}
}

void AMiyamoto_Iori::WeaponUnEquip()
{
	Super::WeaponUnEquip();
	switch (CurSwordStance)
	{
	case ESWORDSTANCE::EST_EARTH:
		break;
	case ESWORDSTANCE::EST_FIRE:
		SecondWeaponComponent->AttachToComponent(BodyComponent,
			FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true),
			FName(TEXT("SecondWeapon")));
		break;
	default:
		break;
	}
}
