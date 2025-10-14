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
	if (OwnerCharacter->GetMovementComponent()->IsFalling() == true ||
		OwnerCharacter->IsEvading() == true ||
		nullptr == HeavyAttackMontage || OwnerCharacter->GetIsCombatMode() == false)
		return;

	//루프 몽타주가 안니거나 강공격 몽타주가 이미 끝났으면 리턴
	if (OwnerCharacter->GetBodyComponent()->GetAnimInstance()->Montage_IsPlaying(HeavyAttackMontage) == false)
		return;

	//몽타주가 플레이 중이고, 연타 횟수가 최대 횟수를 초과하면 리턴
	if (OwnerCharacter->GetBodyComponent()->GetAnimInstance()->Montage_IsPlaying(HeavyAttackMontage) == true &&
		HeavyAttackCount[NormalAttackSectionIndex] > HeavyAttackMaxCount[NormalAttackSectionIndex])
		return;

	IsCharging = false;
	CurrentChargeTime = GetWorld()->GetTimeSeconds() - ChargeStartTime;
	CurrentChargeTime = FMath::Clamp(CurrentChargeTime, 0.0f, MaxChargeTime);
	PlayHeavyAttack0ChargeMontage();
}

void UEarthStanceActorComponent::PlayHeavyAttack0ChargeMontage()
{
	APlayableBaseCharacter* OwnerCharacter = Cast<APlayableBaseCharacter>(GetOwner());
	if (NormalAttackSectionIndex != 0)
		return;
	OwnerCharacter->StopMontage(HeavyAttackMontage);
	//반격 가능 상태면 특수 공격력 추가
	if (OwnerCharacter->GetIsCanConuterAttack() == true)
	{
		SpeicalAttackPower = OwnerCharacter->GetStatusComponent()->GetTotalAttackPower() * 0.05f;
		OwnerCharacter->ResetCounterAttackTimer();
	}
	OwnerCharacter->PlayMontageFullBody(HeavyAttackMontage, HeavyAttackSectionNames[NormalAttackSectionNames.Num() + NormalAttackSectionIndex]);

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("CCCCCPlay Heavy Attack Montage"));
	
	HeavyAttackCount[NormalAttackSectionIndex]++;
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
