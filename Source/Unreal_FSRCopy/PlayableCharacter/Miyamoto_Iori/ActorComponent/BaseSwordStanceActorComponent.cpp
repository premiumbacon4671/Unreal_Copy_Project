// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter/Miyamoto_Iori/ActorComponent/BaseSwordStanceActorComponent.h"
#include "PlayableCharacter/PlayableBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UBaseSwordStanceActorComponent::UBaseSwordStanceActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBaseSwordStanceActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	HeavyAttackCount.Init(0, HeavyAttackMaxCount.Num());
}


// Called every frame
void UBaseSwordStanceActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBaseSwordStanceActorComponent::PlayNormalAttackMontage()
{
	GEngine->AddOnScreenDebugMessage(2, 3.0f, FColor::Blue, TEXT("Play Normal Attack Montage"));
	APlayableBaseCharacter* OwnerCharacter = Cast<APlayableBaseCharacter>(GetOwner());
	if(OwnerCharacter->GetMovementComponent()->IsFalling() == true ||
		nullptr == NormalAttackMontage || OwnerCharacter->GetIsCombatMode() == false ||
		OwnerCharacter->GetBodyComponent()->GetAnimInstance()->Montage_IsPlaying(HeavyAttackMontage) == true ||
		isUseableNormalAttack == false)
		return;

	if (OwnerCharacter->PlayMontageFullBody(NormalAttackMontage, OwnerCharacter->GetAddCurNormalAttackSectionName()) == true)
	{
		isUseableNormalAttack = false;
	}
}

void UBaseSwordStanceActorComponent::PlayHeavyAttackMontage()
{
	APlayableBaseCharacter* OwnerCharacter = Cast<APlayableBaseCharacter>(GetOwner());
	//기본 적인 강공격 가능 여부 확인
	if (OwnerCharacter->GetMovementComponent()->IsFalling() == true ||
		nullptr == HeavyAttackMontage || OwnerCharacter->GetIsCombatMode() == false ||
		isUseableNormalAttack == false ||
		isUseableHeavyAttack == false)
		return;

	//연타 강공격 가능 여부 확인
	int HeavyAttackIndex = OwnerCharacter->GetNormalAttackSectionIndex();
	if (OwnerCharacter->GetBodyComponent()->GetAnimInstance()->Montage_IsPlaying(HeavyAttackMontage) == true &&
		HeavyAttackCount[HeavyAttackIndex] >= HeavyAttackMaxCount[HeavyAttackIndex])
		return;

	GEngine->AddOnScreenDebugMessage(2, 3.0f, FColor::Blue, TEXT("Play Heavy Attack Montage"));
	
	if (OwnerCharacter->PlayMontageFullBody(HeavyAttackMontage, OwnerCharacter->GetCurHeavyAttackSectionName()) == true)
	{
		isUseableHeavyAttack = false;
	}
}

void UBaseSwordStanceActorComponent::PlayTriggeredHeavyAttackMontage()
{
	APlayableBaseCharacter* OwnerCharacter = Cast<APlayableBaseCharacter>(GetOwner());

	if (OwnerCharacter->GetMovementComponent()->IsFalling() == true ||
		nullptr == NormalAttackMontage || OwnerCharacter->GetIsCombatMode() == false ||
		isUseableNormalAttack == false)
		return;

	GEngine->AddOnScreenDebugMessage(3, 3.0f, FColor::Green, TEXT("Play Triggered Heavy Attack Montage"));
	//OwnerCharacter->GetBodyComponent()->GetAnimInstance()->l
}

void UBaseSwordStanceActorComponent::PlayCompletedHeavyAttackMontage()
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

	HeavyAttackCount[HeavyAttackIndex]++;
	GEngine->AddOnScreenDebugMessage(4, 3.0f, FColor::Magenta, TEXT("Play Completed Heavy Attack Montage"));
}

void UBaseSwordStanceActorComponent::ResetNormalAttack()
{
	ResetisUseableNormalAttack(); 
	APlayableBaseCharacter* OwnerCharacter = Cast<APlayableBaseCharacter>(GetOwner());
	OwnerCharacter->ResetNormalAttackSectionIndex();
}

void UBaseSwordStanceActorComponent::ResetHeavyAttack()
{
	ResetisUseableHeavyAttack();
	APlayableBaseCharacter* OwnerCharacter = Cast<APlayableBaseCharacter>(GetOwner());
	OwnerCharacter->ResetNormalAttackSectionIndex();
	for (int i = 0; i < HeavyAttackCount.Num(); ++i)
	{
		HeavyAttackCount[i] = 0;
	}
}

