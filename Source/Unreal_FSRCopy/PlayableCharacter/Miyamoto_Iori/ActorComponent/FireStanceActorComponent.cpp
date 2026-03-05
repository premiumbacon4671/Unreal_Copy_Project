// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter/Miyamoto_Iori/ActorComponent/FireStanceActorComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayableCharacter/PlayableBaseCharacter.h"
#include "PlayableCharacter/Miyamoto_Iori/Miyamoto_Iori.h"
#include "ActorComponent/StateComponent/Miyamoto_ioriStateComponent.h"

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
		HeavyAttackCount[NormalAttackSectionIndex] > HeavyAttackMaxCount[NormalAttackSectionIndex])
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

	if (HeavyAttackCount[ComboAttackIndex] >= HeavyAttackMaxCount[ComboAttackIndex])
	{
		OwnerCharacter->StopMontage(HeavyAttackMontage);
		OwnerCharacter->SetIsActionLock(false);
		return;
	}

	FName TargetSectionName = NAME_None;
	if (HeavyAttackSectionNames.IsValidIndex(ComboAttackIndex))
	{
		TargetSectionName = HeavyAttackSectionNames[ComboAttackIndex];
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Invalid ComboAttackIndex for HeavyAttackSectionNames"));
		return;
	}
	OwnerCharacter->StopMontage(HeavyAttackMontage);
	SpeicalAttackPower = CurrentChargeTime * OwnerCharacter->GetStatusComponent()->GetTotalAttackPower() * 0.2f;
	bool bPlayed = OwnerCharacter->PlayMontageFullBody(HeavyAttackMontage, TargetSectionName, AmountAttackSpeed);
	if (bPlayed)
	{
		NormalAttackSectionIndex = ComboAttackIndex;
		HeavyAttackCount[NormalAttackSectionIndex]++;
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
