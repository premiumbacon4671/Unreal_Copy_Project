// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter/Servant/ServantBaseCharacter.h"
#include "ActorComponent/StateComponent/PlayableStateComponent.h"
#include "PlayableCharacter/Miyamoto_Iori/ActorComponent/BaseSwordStanceActorComponent.h"

AServantBaseCharacter::AServantBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	StatusComponent = CreateDefaultSubobject<UPlayableStateComponent>(TEXT("StatusComponent"));

	FirstWeaponComponent->SetupAttachment(BodyComponent,
		FName(TEXT("FirstWeaponHand")));
}

void AServantBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	CurSwordStanceComponent = Cast<UBaseSwordStanceActorComponent>(GetComponentByClass(UBaseSwordStanceActorComponent::StaticClass()));
	InitializeStatus();
	if (bStartWithWeaponHidden && FirstWeaponComponent)
	{
		FirstWeaponComponent->SetHiddenInGame(true);
	}
}

void AServantBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AServantBaseCharacter::InitializeStatus()
{
	if (!PlayableDataTable)
		return;
	FPlayableStat* SaberStat = PlayableDataTable->FindRow<FPlayableStat>(ServantRowName, TEXT("ServantDataTable"));
	if (nullptr != SaberStat)
	{
		int32 LevelOffset = FMath::Max(0, SaberStat->Level - 1);
		//레벨 비례 스탯 성장 수식 추가 예정
		StatusComponent->InitState(*SaberStat);
	}
}

void AServantBaseCharacter::SetWeaponVisibility(bool bVisible)
{
	if (FirstWeaponComponent)
	{
		FirstWeaponComponent->SetHiddenInGame(bVisible);
	}
}

void AServantBaseCharacter::WeaponEquip()
{
	SetWeaponVisibility(false);
}

void AServantBaseCharacter::WeaponUnEquip()
{
	SetWeaponVisibility(true);
}
