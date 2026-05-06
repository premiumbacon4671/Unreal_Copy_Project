// Fill out your copyright notice in the Description page of Project Settings.


#include "PublicNotify/AttackTrace/AttackTraceAnimNotify.h"
#include "Interface/AttackTraceNotify.h"
#include "PlayableCharacter/PlayableBaseCharacter.h"
#include "Monster/BaseMonster.h"

#include "GameFramework/Character.h"

void UAttackTraceAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp || !MeshComp->GetOwner())
		return;

	//몬스터와 같은 노티파이를 사용하면서 주석
	/*ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (Character != nullptr && Character->GetMesh() != MeshComp)
		return;*/

	IAttackTraceNotify* AttackTraceNotify = Cast<IAttackTraceNotify>(MeshComp->GetOwner());
	if (!AttackTraceNotify)
		return;
	AttackTraceNotify->AttackTrace(AttackVariety);
}
