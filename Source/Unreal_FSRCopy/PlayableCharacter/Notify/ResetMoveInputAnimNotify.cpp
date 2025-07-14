// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter/Notify/ResetMoveInputAnimNotify.h"

void UResetMoveInputAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp)
	{
		APawn* Owner = Cast<APawn>(MeshComp->GetOwner());
		if (Owner && Owner->GetController())
			Owner->GetController()->ResetIgnoreMoveInput();
	}
}
