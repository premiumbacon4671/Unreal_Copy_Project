// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter/Miyamoto_Iori/Animation/MiyamotoIoriAnimInstance.h"
#include "PlayableCharacter/PlayableBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Controller/MiyamotoIoriController/MiyamotoIoriController.h"
#include "PlayableCharacter/Miyamoto_Iori/Miyamoto_Iori.h"
#include "PlayableCharacter/Miyamoto_Iori/ActorComponent/BaseSwordStanceActorComponent.h"

void UMiyamotoIoriAnimInstance::NativeInitializeAnimation()
{
	OwnerCharacter = Cast<APlayableBaseCharacter>(TryGetPawnOwner());
	if (OwnerCharacter)
	{
		MovementComponent = OwnerCharacter->GetCharacterMovement();
	}
}

void UMiyamotoIoriAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (OwnerCharacter)
	{
		isMove = MovementComponent->Velocity.Size() > 0.0f;
		MoveVelocity = MovementComponent->Velocity.Size2D();
		isJump = OwnerCharacter->GetMovementComponent()->IsFalling();
		AMiyamotoIoriController* OwnerController = Cast<AMiyamotoIoriController>(OwnerCharacter->GetController());
		isEquip = OwnerCharacter->GetIsCombatMode();
		CurSwordStance = Cast<AMiyamoto_Iori>(OwnerCharacter)->GetCurSwordStance();
		if (nullptr != OwnerController)
			isMoveInput = OwnerController->GetIsMoveInput();
	}
}

UAnimSequence* UMiyamotoIoriAnimInstance::GetCombatIdleSequence() const
{
	if (nullptr != OwnerCharacter)
	{
		UBaseSwordStanceActorComponent* CurStanceComponent = Cast<AMiyamoto_Iori>(OwnerCharacter)->GetCurSwordStanceComponent();
		if (nullptr != CurStanceComponent)
		{
			return CurStanceComponent->GetIdleSequenece();
		}
	}
	return nullptr;
}
