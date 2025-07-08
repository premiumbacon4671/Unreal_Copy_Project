// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter/Miyamoto_Iori/Animation/MiyamotoIoriAnimInstance.h"
#include "PlayableCharacter/PlayableBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Controller/MiyamotoIoriController/MiyamotoIoriController.h"

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
		//isEquip = OwnerCharacter->GetisEquip();
		if (nullptr != OwnerController)
			isMoveInput = OwnerController->GetIsMoveInput();
	}
}
