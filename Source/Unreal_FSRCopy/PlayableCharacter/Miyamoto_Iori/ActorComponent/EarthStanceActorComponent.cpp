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
	//콤보 공격 인덱스 가져오기
	int32 ComboAttackIndex;
	if (ComboAttackIndexMap.Contains(NormalAttackSectionIndex) == true)
	{
		ComboAttackIndex = ComboAttackIndexMap[NormalAttackSectionIndex];
	}
	else
		return;
	//콤보 공격 인덱스가 유효한지 확인
	if(HeavyAttackData.IsValidIndex(ComboAttackIndex) == false)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Invalid ComboAttackIndex"));
		return;
	}
	//콤보 공격 최대 횟수에 도달했는지 확인
	if(HeavyAttackData[ComboAttackIndex].AttackCount >= HeavyAttackData[ComboAttackIndex].AttackMaxCount)
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
	//콤보 공격 섹션 이름 가져오기
	FName TargetSectionName = NAME_None;
	if(HeavyAttackData[ComboAttackIndex].MontageSectionName != NAME_None)
	{
		TargetSectionName = HeavyAttackData[ComboAttackIndex].MontageSectionName;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Invalid ComboAttackIndex for HeavyAttackSectionNames"));
		return;
	}
	//다음 공격 몽타주 재생
	OwnerCharacter->StopMontage(HeavyAttackMontage);
	bool bPlayed = OwnerCharacter->PlayMontageFullBody(HeavyAttackMontage, TargetSectionName, AmountAttackSpeed);
	if(bPlayed == true)
	{
		NormalAttackSectionIndex = ComboAttackIndex;
		HeavyAttackData[NormalAttackSectionIndex].AttackCount++;
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
