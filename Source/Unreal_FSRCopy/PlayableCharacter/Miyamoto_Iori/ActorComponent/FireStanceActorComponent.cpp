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

	PlayHeavyAttack0ChargeMontage();
}

void UFireStanceActorComponent::PlayHeavyAttack0ChargeMontage()
{
	APlayableBaseCharacter* OwnerCharacter = Cast<APlayableBaseCharacter>(GetOwner());
	if (NormalAttackSectionIndex != 0)
		return;
	OwnerCharacter->StopMontage(HeavyAttackMontage);
	OwnerCharacter->PlayMontageFullBody(HeavyAttackMontage, HeavyAttackSectionNames[NormalAttackSectionNames.Num() + NormalAttackSectionIndex]);

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("CCCCCPlay Heavy Attack Montage"));
	HeavyAttackCount[NormalAttackSectionIndex]++;
}

void UFireStanceActorComponent::ReleaseSwordStance()
{
	Super::ReleaseSwordStance();
	AddAttackSpeed = 0.0f;
	AddAttackPower = 0;
}

void UFireStanceActorComponent::InitSwordStance()
{
	Super::InitSwordStance();
	SetAddStanceaAility();
}

int UFireStanceActorComponent::SwordStanceBeforeUpdateHp(int Damage)
{
	Super::SwordStanceBeforeUpdateHp(Damage);
	return Damage;
}

void UFireStanceActorComponent::SwordStanceAfterUpdateHp(int Damage)
{
	Super::SwordStanceAfterUpdateHp(Damage);
	SetAddStanceaAility();
}

void UFireStanceActorComponent::SwordStanceUpdateAttack()
{
	Super::SwordStanceUpdateAttack();
}

void UFireStanceActorComponent::SetAddStanceaAility()
{
	AMiyamoto_Iori* OwnerCharacter = Cast<AMiyamoto_Iori>(GetOwner());
	if (OwnerCharacter)
	{
		float HpPercent = OwnerCharacter->GetStatusComponent()->GetHPPercent();
		//Hp 70% or AttackSpeed 30%
		if (HpPercent > 0.7 || AddAttackSpeed >= 0.35)
			return;
		float LostHpPercent = 1.0f - HpPercent;

		AddAttackSpeed = LostHpPercent * 0.5f;
		AddAttackPower = static_cast<int>(OwnerCharacter->GetStatusComponent()->GetAttackPower() * LostHpPercent * 0.3f);

		if(AddAttackSpeed >= 0.35f)
			AddAttackSpeed = 0.35f;
	}
}
