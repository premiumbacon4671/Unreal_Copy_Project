// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter/Notify/Attack/CheckNextAttackAnimNotifyState.h"
#include "PlayableCharacter/PlayableBaseCharacter.h"
#include "PlayableCharacter/Miyamoto_Iori/ActorComponent/BaseSwordStanceActorComponent.h"

bool UCheckNextAttackAnimNotifyState::Received_NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) const
{
	APlayableBaseCharacter* Owner = Cast<APlayableBaseCharacter>(MeshComp->GetOwner());
	if (nullptr != Owner)
		return false;
	Owner->GetCurSwordStanceComponent()->CanIsInputPressed();
	return true;
}

bool UCheckNextAttackAnimNotifyState::Received_NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) const
{
	APlayableBaseCharacter* Owner = Cast<APlayableBaseCharacter>(MeshComp->GetOwner());
	if (nullptr != Owner)
		return false;
	Owner->GetCurSwordStanceComponent()->ResetIsInputPressed();
	return true;
}
