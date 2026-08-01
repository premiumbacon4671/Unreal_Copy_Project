// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter/Notify/Skill/GatherEnemiesAnimNotify.h"

#include "PlayableCharacter/PlayableBaseCharacter.h"
#include "ActorComponent/SkillActionComponent/SkillActionComponent.h"
#include "DataAsset/PrimaryDataAsset/SkillDataAsset/SkillDataAsset.h"

void UGatherEnemiesAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	APlayableBaseCharacter* Owner = Cast<APlayableBaseCharacter>(MeshComp->GetOwner());
	if (nullptr != Owner)
	{
		if(Owner->GetSkillActionComponent() != nullptr)
			Owner->GetSkillActionComponent()->GatherEnemies();
	}
}
