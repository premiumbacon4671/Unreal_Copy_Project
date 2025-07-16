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
		OwnerCharacter->GetBodyComponent()->GetAnimInstance()->Montage_IsPlaying(NormalAttackMontage) == true ||
		OwnerCharacter->GetBodyComponent()->GetAnimInstance()->Montage_IsPlaying(HeavyAttackMontage) == true)
		return;
	OwnerCharacter->PlayMontageFullBody(NormalAttackMontage, OwnerCharacter->GetAddCurNormalAttackSectionName());
}

void UBaseSwordStanceActorComponent::PlayHeavyAttackMontage()
{
	GEngine->AddOnScreenDebugMessage(2, 3.0f, FColor::Blue, TEXT("Play Normal Attack Montage"));
	APlayableBaseCharacter* OwnerCharacter = Cast<APlayableBaseCharacter>(GetOwner());
	if (OwnerCharacter->GetMovementComponent()->IsFalling() == true ||
		nullptr == NormalAttackMontage || OwnerCharacter->GetIsCombatMode() == false ||
		OwnerCharacter->GetBodyComponent()->GetAnimInstance()->Montage_IsPlaying(NormalAttackMontage) == true ||
		OwnerCharacter->GetBodyComponent()->GetAnimInstance()->Montage_IsPlaying(HeavyAttackMontage) == true)
		return;
	OwnerCharacter->PlayMontageFullBody(HeavyAttackMontage, OwnerCharacter->GetCurHeavyAttackSectionName());
}

void UBaseSwordStanceActorComponent::PlayTriggeredHeavyAttackMontage()
{
}

