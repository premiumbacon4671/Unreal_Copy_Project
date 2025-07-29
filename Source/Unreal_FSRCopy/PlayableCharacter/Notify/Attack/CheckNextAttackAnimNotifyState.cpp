// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter/Notify/Attack/CheckNextAttackAnimNotifyState.h"
#include "PlayableCharacter/PlayableBaseCharacter.h"
#include "PlayableCharacter/Miyamoto_Iori/ActorComponent/BaseSwordStanceActorComponent.h"

void UCheckNextAttackAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	APlayableBaseCharacter* Owner = Cast<APlayableBaseCharacter>(MeshComp->GetOwner());
	if (nullptr == Owner)
		return;
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("NotifyBegin"));
	Owner->GetCurSwordStanceComponent()->ResetNextAttack();
}

void UCheckNextAttackAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	APlayableBaseCharacter* Owner = Cast<APlayableBaseCharacter>(MeshComp->GetOwner());
	if (nullptr == Owner)
		return;
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("NotifyEnd"));
	Owner->GetCurSwordStanceComponent()->PlayNextAttackMontage();
}
