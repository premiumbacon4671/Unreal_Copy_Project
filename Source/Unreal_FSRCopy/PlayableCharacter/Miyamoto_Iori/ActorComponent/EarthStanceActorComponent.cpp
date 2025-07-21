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
		nullptr == HeavyAttackMontage || OwnerCharacter->GetIsCombatMode() == false ||
		OwnerCharacter->GetBodyComponent()->GetAnimInstance()->Montage_IsPlaying(NormalAttackMontage) == true ||
		isUseableHeavyAttack == true)
		return;

	int HeavyAttackIndex = OwnerCharacter->GetNormalAttackSectionIndex();
	GEngine->AddOnScreenDebugMessage(6, 3.0f, FColor::Magenta, FString::FromInt(HeavyAttackCount[HeavyAttackIndex]));
	if (OwnerCharacter->GetBodyComponent()->GetAnimInstance()->Montage_IsPlaying(HeavyAttackMontage) == true &&
		HeavyAttackCount[HeavyAttackIndex] > 0 &&
		HeavyAttackCount[HeavyAttackIndex] >= HeavyAttackMaxCount[HeavyAttackIndex])
		return;

	GEngine->AddOnScreenDebugMessage(4, 3.0f, FColor::Magenta, TEXT("Play Earth Completed Heavy Attack Montage"));
	PlayHeavyAttack0ChargeMontage();
	HeavyAttackCount[HeavyAttackIndex]++;
}

void UEarthStanceActorComponent::PlayHeavyAttack0ChargeMontage()
{
	APlayableBaseCharacter* OwnerCharacter = Cast<APlayableBaseCharacter>(GetOwner());

	if (OwnerCharacter->GetNormalAttackSectionIndex() != 0)
		return;

	GEngine->AddOnScreenDebugMessage(5, 3.0f, FColor::Magenta, TEXT("Play Earth Charge Heavy Attack0 Montage"));
	//OwnerCharacter->StopMontage(HeavyAttackMontage);
	OwnerCharacter->PlayMontageFullBody(HeavyAttackMontage, ExtraHeavyAttackNames[OwnerCharacter->GetNormalAttackSectionIndex()]);
}
