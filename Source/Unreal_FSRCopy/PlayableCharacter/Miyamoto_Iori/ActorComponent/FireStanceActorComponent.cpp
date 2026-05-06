// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter/Miyamoto_Iori/ActorComponent/FireStanceActorComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "PlayableCharacter/PlayableBaseCharacter.h"
#include "PlayableCharacter/Miyamoto_Iori/Miyamoto_Iori.h"
#include "ActorComponent/StateComponent/Miyamoto_ioriStateComponent.h"
#include "PublicUse/AttackCombatStruct/AttackCombatStruct.h"

UFireStanceActorComponent::UFireStanceActorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> NormalAttackMontageFinder(
		TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/PlayableCharacter/MiyamotoIori/Animation/AM_FireNormalAttack.AM_FireNormalAttack'"));
	if (NormalAttackMontageFinder.Succeeded())
		NormalAttackMontage = NormalAttackMontageFinder.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HeavyAttackMontageFinder(
		TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/PlayableCharacter/MiyamotoIori/Animation/AM_FireHeavyAttack.AM_FireHeavyAttack'"));
	if (HeavyAttackMontageFinder.Succeeded())
		HeavyAttackMontage = HeavyAttackMontageFinder.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> CounterAttackMontageFinder(
		TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/PlayableCharacter/MiyamotoIori/Animation/AM_TwoHandsCounterAttaack.AM_TwoHandsCounterAttaack'"));
	if (CounterAttackMontageFinder.Succeeded())
		CounterAttackMontage = CounterAttackMontageFinder.Object;
}


void UFireStanceActorComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UFireStanceActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UFireStanceActorComponent::PlayCompletedHeavyAttackMontage()
{
	APlayableBaseCharacter* OwnerCharacter = Cast<APlayableBaseCharacter>(GetOwner());
	if (OwnerCharacter->GetMovementComponent()->IsFalling() == true ||
		OwnerCharacter->IsEvading() == true ||
		nullptr == HeavyAttackMontage || OwnerCharacter->GetIsCombatMode() == false)
		return;

	//루프 몽타주가 안니거나 강공격 몽타주가 이미 끝났으면 린턴
	if (OwnerCharacter->GetBodyComponent()->GetAnimInstance()->Montage_IsPlaying(HeavyAttackMontage) == false)
		return;

	//몽타주가 플레이 중이고, 연타 횟수가 최대 횟수를 초과하면 리턴
	if (OwnerCharacter->GetBodyComponent()->GetAnimInstance()->Montage_IsPlaying(HeavyAttackMontage) == true &&
		HeavyAttackData[NormalAttackSectionIndex].AttackCount > HeavyAttackData[NormalAttackSectionIndex].AttackMaxCount)
		return;

	IsCharging = false;
	CurrentChargeTime = GetWorld()->GetTimeSeconds() - ChargeStartTime;
	CurrentChargeTime = FMath::Clamp(CurrentChargeTime, 0.0f, MaxChargeTime);
	PlayHeavyAttack0ChargeMontage();
}

void UFireStanceActorComponent::PlayHeavyAttack0ChargeMontage()
{
	APlayableBaseCharacter* OwnerCharacter = Cast<APlayableBaseCharacter>(GetOwner());
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
	if (HeavyAttackData.IsValidIndex(ComboAttackIndex) == false)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Invalid ComboAttackIndex"));
		return;
	}
	//콤보 공격 최대 횟수에 도달했는지 확인
	if (HeavyAttackData[ComboAttackIndex].AttackCount >= HeavyAttackData[ComboAttackIndex].AttackMaxCount)
	{
		OwnerCharacter->StopMontage(HeavyAttackMontage);
		OwnerCharacter->SetIsActionLock(false);
		return;
	}
	//콤보 공격 섹션 이름 가져오기
	FName TargetSectionName = NAME_None;
	if (HeavyAttackData.IsValidIndex(ComboAttackIndex) == true)
	{
		TargetSectionName = HeavyAttackData[ComboAttackIndex].MontageSectionName;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Invalid ComboAttackIndex for HeavyAttackSectionNames"));
		return;
	}
	//몽타주 플레이
	OwnerCharacter->StopMontage(HeavyAttackMontage);
	SpeicalAttackPower = CurrentChargeTime * OwnerCharacter->GetStatusComponent()->GetTotalAttackPower() * 0.2f;
	bool bPlayed = OwnerCharacter->PlayMontageFullBody(HeavyAttackMontage, TargetSectionName, AmountAttackSpeed);
	if (bPlayed)
	{
		NormalAttackSectionIndex = ComboAttackIndex;
		HeavyAttackData[ComboAttackIndex].AttackCount++;
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, FString::Printf(TEXT("Charge Time : %f"), CurrentChargeTime));
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, FString::Printf(TEXT("ChargeStartTime : %f"), ChargeStartTime));
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, FString::Printf(TEXT("SpeicalAttackPower : %d"), SpeicalAttackPower));
	}
	else
	{
		OwnerCharacter->SetIsActionLock(false);
	}
}

void UFireStanceActorComponent::ReleaseSwordStance()
{
	Super::ReleaseSwordStance();
	ExtraAttackSpeed = 0.0f;
	UPlayableStateComponent* StateComp = Cast<APlayableBaseCharacter>(GetOwner())->GetStatusComponent();
	StateComp->SubExtraAttackPower(ExtraAttackPower);
	ExtraAttackPower = 0;
}

void UFireStanceActorComponent::InitSwordStance()
{
	Super::InitSwordStance();
	SetExtraStanceaAility();
}

int UFireStanceActorComponent::SwordStanceBeforeUpdateHp(int Damage)
{
	Super::SwordStanceBeforeUpdateHp(Damage);
	return Damage;
}

void UFireStanceActorComponent::SwordStanceAfterUpdateHp(int Damage)
{
	Super::SwordStanceAfterUpdateHp(Damage);
	SetExtraStanceaAility();
}

int UFireStanceActorComponent::SwordStanceUpdateAttack()
{
	int UpdateAttackValue;
	UpdateAttackValue = Super::SwordStanceUpdateAttack();
	return UpdateAttackValue;
}

void UFireStanceActorComponent::SetExtraStanceaAility()
{
	AMiyamoto_Iori* OwnerCharacter = Cast<AMiyamoto_Iori>(GetOwner());
	if (OwnerCharacter)
	{
		UPlayableStateComponent* StateComp = OwnerCharacter->GetStatusComponent();
		float HpPercent = StateComp->GetHPPercent();
		//Hp 70% or AttackSpeed 30%
		if (HpPercent > 0.7 || ExtraAttackSpeed >= 0.35)
			return;
		float LostHpPercent = 1.0f - HpPercent;
		ExtraAttackSpeed = LostHpPercent * 0.5f;
		StateComp->SubExtraAttackPower(ExtraAttackPower);
		ExtraAttackPower = static_cast<int>(StateComp->GetAttackPower() * LostHpPercent * 0.3f);
		StateComp->AddExtraAttackPower(ExtraAttackPower);

		if(ExtraAttackSpeed >= 0.35f)
			ExtraAttackSpeed = 0.35f;

		AmountAttackSpeed = BasicAttackSpeed + ExtraAttackSpeed;
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("AddAttackSpeed : %f"), ExtraAttackSpeed));
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("AddAttackPower : %d"), ExtraAttackPower));
	}
}
