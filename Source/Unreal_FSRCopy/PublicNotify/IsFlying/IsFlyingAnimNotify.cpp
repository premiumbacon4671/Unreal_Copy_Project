// Fill out your copyright notice in the Description page of Project Settings.


#include "PublicNotify/IsFlying/IsFlyingAnimNotify.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UIsFlyingAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	if(nullptr != character)
	{
		character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	}
}
