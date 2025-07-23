// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter/Notify/Attack/ResetHeavyAttackAnimNotify.h"
#include "PlayableCharacter/PlayableBaseCharacter.h"
#include "PlayableCharacter/Miyamoto_Iori/ActorComponent/BaseSwordStanceActorComponent.h"

void UResetHeavyAttackAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	APlayableBaseCharacter* Owner = Cast<APlayableBaseCharacter>(MeshComp->GetOwner());
	if (nullptr != Owner)
	{
		if (Owner->GetCurSwordStanceComponent())
		{

		}
			//Owner->GetCurSwordStanceComponent()->ResetHeavyAttack();
	}
}