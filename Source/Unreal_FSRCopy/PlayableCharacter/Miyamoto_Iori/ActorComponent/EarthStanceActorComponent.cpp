// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter/Miyamoto_Iori/ActorComponent/EarthStanceActorComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayableCharacter/PlayableBaseCharacter.h"
#include "ActorComponent/StateComponent/PlayableStateComponent.h"

UEarthStanceActorComponent::UEarthStanceActorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> NormalAttackMontageFinder(
		TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/PlayableCharacter/MiyamotoIori/Animation/AM_EarthNormalAttack.AM_EarthNormalAttack'"));
	if (NormalAttackMontageFinder.Succeeded())
		NormalAttackMontage = NormalAttackMontageFinder.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HeavyAttackMontageFinder(
		TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/PlayableCharacter/MiyamotoIori/Animation/AM_EarthHeavyAttack.AM_EarthHeavyAttack'"));
	if (HeavyAttackMontageFinder.Succeeded())
		HeavyAttackMontage = HeavyAttackMontageFinder.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> CounterAttackMontageFinder(
		TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/PlayableCharacter/MiyamotoIori/Animation/AM_OneHandCounterAttack.AM_OneHandCounterAttack'"));
	if (CounterAttackMontageFinder.Succeeded())
		CounterAttackMontage = CounterAttackMontageFinder.Object;
}

void UEarthStanceActorComponent::BeginPlay()
{
	Super::BeginPlay();
	EarthStanceShield = MaxEarthStanceShield;
}

void UEarthStanceActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UEarthStanceActorComponent::PlayCompletedHeavyAttackMontage()
{
	APlayableBaseCharacter* OwnerCharacter = Cast<APlayableBaseCharacter>(GetOwner());
	if (!IsValid(OwnerCharacter))
		return;

	if (IsCharging == false)
		return;

	UAnimInstance* AnimInstance = OwnerCharacter->GetBodyComponent()->GetAnimInstance();
	if (!IsValid(AnimInstance))
		return;

	//루프 몽타주가 안니거나 강공격 몽타주가 이미 끝났으면 리턴
	if (AnimInstance->Montage_IsPlaying(HeavyAttackMontage) == false)
	{
		IsCharging = false;
		return;
	}

	IsCharging = false;
	CurrentChargeTime = GetWorld()->GetTimeSeconds() - ChargeStartTime;
	CurrentChargeTime = FMath::Clamp(CurrentChargeTime, 0.0f, MaxChargeTime);
	PlayHeavyAttack0ChargeMontage();
}

void UEarthStanceActorComponent::PlayHeavyAttack0ChargeMontage()
{
	APlayableBaseCharacter* OwnerCharacter = Cast<APlayableBaseCharacter>(GetOwner());
	if (!IsValid(OwnerCharacter))
		return;

	if (NormalAttackSectionIndex != 0)
		return;

	if (ComboAttackIndexMap.Contains(NormalAttackSectionIndex) == true)
	{
		int32 ComboAttackIndex = ComboAttackIndexMap[NormalAttackSectionIndex];
	}
	else
		return;
	int32 ComboAttackIndex = ComboAttackIndexMap[NormalAttackSectionIndex];
	if (!HeavyAttackCount.IsValidIndex(ComboAttackIndex)
		|| !HeavyAttackMaxCount.IsValidIndex(ComboAttackIndex))
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Invalid ComboAttackIndex"));
		return;
	}

	if(HeavyAttackCount[ComboAttackIndex] >= HeavyAttackMaxCount[ComboAttackIndex])
	{
		OwnerCharacter->StopMontage(HeavyAttackMontage);
		OwnerCharacter->SetIsActionLock(false);
		return;
	}

	//반격 보너스
	bool bSuccessCounter = OwnerCharacter->GetIsCanGuardConuterAttack();
	if (bSuccessCounter)
	{
		SpeicalAttackPower = OwnerCharacter->GetStatusComponent()->GetTotalAttackPower() * 0.05f;
		OwnerCharacter->ResetCounterAttackTimer();
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, TEXT("!!! Success Counter Attack !!!"));
	}
	else
	{
		SpeicalAttackPower = 0.0f;
	}

	FName TargetSectionName = NAME_None;
	if(HeavyAttackSectionNames.IsValidIndex(ComboAttackIndex))
	{
		TargetSectionName = HeavyAttackSectionNames[ComboAttackIndex];
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Invalid ComboAttackIndex for HeavyAttackSectionNames"));
		return;
	}
	OwnerCharacter->StopMontage(HeavyAttackMontage);
	bool bPlayed = OwnerCharacter->PlayMontageFullBody(HeavyAttackMontage, TargetSectionName, AmountAttackSpeed);
	if(bPlayed == true)
	{
		NormalAttackSectionIndex = ComboAttackIndex;
		HeavyAttackCount[NormalAttackSectionIndex]++;
	}
	else
	{
		OwnerCharacter->SetIsActionLock(false);
	}
}

void UEarthStanceActorComponent::ReleaseSwordStance()
{
	Super::ReleaseSwordStance();
	//방패 재충전 타이머 생성
	if(GetWorld()->GetTimerManager().IsTimerActive(ShieldRechargeTimer) == false)
	{
		GetWorld()->GetTimerManager().SetTimer(
			ShieldRechargeTimer,
			this,
			&UEarthStanceActorComponent::RechargeEarthStanceShield,
			60.0f, false);
	}
	else
	{
		GetWorld()->GetTimerManager().UnPauseTimer(ShieldRechargeTimer);
	}
	UPlayableStateComponent* OwnerCharacterStatus = Cast<APlayableBaseCharacter>(GetOwner())->GetStatusComponent();
	if (OwnerCharacterStatus == nullptr)
		return;
	OwnerCharacterStatus->SubExtraDefencePower(ExtraDefencePower);
	ExtraDefencePower = 0;
}

void UEarthStanceActorComponent::InitSwordStance()
{
	Super::InitSwordStance();
	//방패 재충전 타이머 정지
	if (GetWorld()->GetTimerManager().IsTimerActive(ShieldRechargeTimer) == true)
	{
		GetWorld()->GetTimerManager().PauseTimer(ShieldRechargeTimer);
	}
	UPlayableStateComponent* OwnerCharacterStatus = Cast<APlayableBaseCharacter>(GetOwner())->GetStatusComponent();
	if(OwnerCharacterStatus == nullptr)
		return;
	ExtraDefencePower = OwnerCharacterStatus->GetDefencePower() * ExtraDefencePowerPercent;
	OwnerCharacterStatus->AddExtraDefencePower(ExtraDefencePower);
}

void UEarthStanceActorComponent::RechargeEarthStanceShield()
{
	EarthStanceShield = MaxEarthStanceShield;
}

int UEarthStanceActorComponent::SwordStanceBeforeUpdateHp(int Damage)
{
	Damage = Super::SwordStanceBeforeUpdateHp(Damage); 
	if (EarthStanceShield > 0)
	{
		EarthStanceShield -= Damage;
		if(EarthStanceShield < 0)
			EarthStanceShield = 0;
		Damage = 0;
	}
	return Damage;
}

void UEarthStanceActorComponent::SwordStanceAfterUpdateHp(int Damage)
{
	Super::SwordStanceAfterUpdateHp(Damage);
}

int UEarthStanceActorComponent::SwordStanceUpdateAttack()
{
	int SuperValue = Super::SwordStanceUpdateAttack();
	return SuperValue;
}
