// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayableCharacter/Notify/Skill/SkillCinematicAnimNotifyState.h"
#include "ActorComponent/SkillActionComponent/SkillActionComponent.h"

void USkillCinematicAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if(nullptr == TargetSkill)
		return;
	if(MeshComp && MeshComp->GetOwner())
	{
		if(USkillActionComponent* SkillActionComp = MeshComp->GetOwner()->FindComponentByClass<USkillActionComponent>())
		{
			SkillActionComp->PlaySkillCinematic(TargetSkill);
		}
	}
}

void USkillCinematicAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if(nullptr == TargetSkill)
		return;
	if(MeshComp && MeshComp->GetOwner())
	{
		if(USkillActionComponent* SkillActionComp = MeshComp->GetOwner()->FindComponentByClass<USkillActionComponent>())
		{
			SkillActionComp->EndSkillCinematic(TargetSkill);
		}
	}
}