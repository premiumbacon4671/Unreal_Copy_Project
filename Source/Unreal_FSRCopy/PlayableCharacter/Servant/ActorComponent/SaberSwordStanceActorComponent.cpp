// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter/Servant/ActorComponent/SaberSwordStanceActorComponent.h"

USaberSwordStanceActorComponent::USaberSwordStanceActorComponent()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> NormalAttackMontageFinder(
		TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/PlayableCharacter/Saber/Animation/AM_SaberNormalAttack.AM_SaberNormalAttack'"));
	if (NormalAttackMontageFinder.Succeeded())
		NormalAttackMontage = NormalAttackMontageFinder.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HeavyAttackMontageFinder(
		TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/PlayableCharacter/Saber/Animation/AM_SaberHeavyAttack.AM_SaberHeavyAttack'"));
	if (HeavyAttackMontageFinder.Succeeded())
		HeavyAttackMontage = HeavyAttackMontageFinder.Object;

	//리소스 부족으로 이오리의 것을 같이 사용함
	static ConstructorHelpers::FObjectFinder<UAnimMontage> CounterAttackMontageFinder(
		TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/PlayableCharacter/MiyamotoIori/Animation/AM_TwoHandsCounterAttaack.AM_TwoHandsCounterAttaack'"));
	if (CounterAttackMontageFinder.Succeeded())
		CounterAttackMontage = CounterAttackMontageFinder.Object;
	WeaponVFXTarget = EWeaponVFXTarget::RightWeapon;
}

void USaberSwordStanceActorComponent::BeginPlay()
{
	Super::BeginPlay();
}

