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

	GEngine->AddOnScreenDebugMessage(2, 0.f, FColor::Yellow, FString::Printf(TEXT("Saber ActionLock : %s"), GetIsActionLock() ? TEXT("true") : TEXT("false")));
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

bool AServantBaseCharacter::CanForceSwap() const
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AnimInstance->IsAnyMontagePlaying())
	{
		UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage();
		//회피, 점프 몽타주일 경우 교체 가능
		if (CurrentMontage == EvadeMontage || CurrentMontage == JumpMontage)
		{
			return true;
		}

		//공격, 스킬 몽타주일 경우 교체 불가
		return false;
	}
	return true;
}
