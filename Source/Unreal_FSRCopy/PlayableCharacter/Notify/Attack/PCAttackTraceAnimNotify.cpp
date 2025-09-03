// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter/Notify/Attack/PCAttackTraceAnimNotify.h"
#include "Interface/AttackTraceNotify.h"
#include "GameFramework/Character.h"

void UPCAttackTraceAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if(!MeshComp || !MeshComp->GetOwner())
		return;
	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	if(Character != nullptr && Character->GetMesh() != MeshComp)
		return;

	IAttackTraceNotify* AttackTraceNotify = Cast<IAttackTraceNotify>(MeshComp->GetOwner());
	if(!AttackTraceNotify)
		return;
	AttackTraceNotify->AttackTrace();
}
