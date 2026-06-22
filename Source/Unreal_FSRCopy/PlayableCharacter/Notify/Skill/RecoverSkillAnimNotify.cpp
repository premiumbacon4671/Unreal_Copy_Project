// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter/Notify/Skill/RecoverSkillAnimNotify.h"
#include "ActorComponent/SkillActionComponent/SkillActionComponent.h"
#include "DataAsset/PrimaryDataAsset/SkillDataAsset/SkillDataAsset.h"

void URecoverSkillAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp)
		return;
	AActor* OwnerActor = MeshComp->GetOwner();
	if (!OwnerActor)
		return;
	USkillActionComponent* SkillActionComp = OwnerActor->FindComponentByClass<USkillActionComponent>();
	if (SkillActionComp)
	{
		USkillDataAsset* SkillData = SkillActionComp->GetCurrentSKillData();
		SkillActionComp->FireRecoverSkill(SkillData);
	}
}
