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

	GEngine->AddOnScreenDebugMessage(4, 3.0f, FColor::Magenta, TEXT("1111 Play Earth Completed Heavy Attack Montage"));
	Super::PlayCompletedHeavyAttackMontage();
	APlayableBaseCharacter* OwnerCharacter = Cast<APlayableBaseCharacter>(GetOwner());
	if (OwnerCharacter->GetMovementComponent()->IsFalling() == true ||
		OwnerCharacter->IsEvading() == true ||
		nullptr == HeavyAttackMontage || OwnerCharacter->GetIsCombatMode() == false)
		return;

	GEngine->AddOnScreenDebugMessage(4, 3.0f, FColor::Magenta, TEXT("2222 Play Earth Completed Heavy Attack Montage"));
	GEngine->AddOnScreenDebugMessage(6, 3.0f, FColor::Magenta, FString::FromInt(HeavyAttackCount[NormalAttackSectionIndex]));
	if (OwnerCharacter->GetBodyComponent()->GetAnimInstance()->Montage_IsPlaying(HeavyAttackMontage) == true &&
		HeavyAttackCount[NormalAttackSectionIndex] > 0 &&
		HeavyAttackCount[NormalAttackSectionIndex] >= HeavyAttackMaxCount[NormalAttackSectionIndex])
		return;

	GEngine->AddOnScreenDebugMessage(4, 3.0f, FColor::Magenta, TEXT("3333 Play Earth Completed Heavy Attack Montage"));
	HeavyAttackCount[NormalAttackSectionIndex]++;
	PlayHeavyAttack0ChargeMontage();
}

void UEarthStanceActorComponent::PlayHeavyAttack0ChargeMontage()
{
	APlayableBaseCharacter* OwnerCharacter = Cast<APlayableBaseCharacter>(GetOwner());

	if (NormalAttackSectionIndex != 0)
		return;

	GEngine->AddOnScreenDebugMessage(5, 3.0f, FColor::Magenta, TEXT("Test") + HeavyAttackSectionNames[NormalAttackSectionNames.Num() + NormalAttackSectionIndex].ToString());
	OwnerCharacter->StopMontage(HeavyAttackMontage);
	OwnerCharacter->PlayMontageFullBody(HeavyAttackMontage, HeavyAttackSectionNames[NormalAttackSectionNames.Num() + NormalAttackSectionIndex]);
}
