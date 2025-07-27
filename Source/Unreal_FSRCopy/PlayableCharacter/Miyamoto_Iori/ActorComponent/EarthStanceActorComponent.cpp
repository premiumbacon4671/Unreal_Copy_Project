// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter/Miyamoto_Iori/ActorComponent/EarthStanceActorComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayableCharacter/PlayableBaseCharacter.h"

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
		nullptr == HeavyAttackMontage || OwnerCharacter->GetIsCombatMode() == false ||
		IsPlayingNormalAttack == true ||
		IsPlayingHeavyAttack == false)
		//isUseableHeavyAttack == true)
		return;

	GEngine->AddOnScreenDebugMessage(6, 3.0f, FColor::Magenta, FString::FromInt(HeavyAttackCount[NormalAttackSectionIndex]));
	if (IsPlayingHeavyAttack == true &&
		HeavyAttackCount[NormalAttackSectionIndex] > 0 &&
		HeavyAttackCount[NormalAttackSectionIndex] >= HeavyAttackMaxCount[NormalAttackSectionIndex])
		return;

	//GEngine->AddOnScreenDebugMessage(4, 3.0f, FColor::Magenta, TEXT("Play Earth Completed Heavy Attack Montage"));
	PlayHeavyAttack0ChargeMontage();
	HeavyAttackCount[NormalAttackSectionIndex]++;
}

void UEarthStanceActorComponent::PlayHeavyAttack0ChargeMontage()
{
	APlayableBaseCharacter* OwnerCharacter = Cast<APlayableBaseCharacter>(GetOwner());

	if (NormalAttackSectionIndex != 0)
		return;

	GEngine->AddOnScreenDebugMessage(5, 3.0f, FColor::Magenta, TEXT("Test") + HeavyAttackSectionNames[NormalAttackSectionNames.Num() + NormalAttackSectionIndex].ToString());
	OwnerCharacter->StopMontage(HeavyAttackMontage);
	IsPlayingHeavyAttack = false;
	OwnerCharacter->PlayMontageFullBody(HeavyAttackMontage, HeavyAttackSectionNames[NormalAttackSectionNames.Num() + NormalAttackSectionIndex]);
}
