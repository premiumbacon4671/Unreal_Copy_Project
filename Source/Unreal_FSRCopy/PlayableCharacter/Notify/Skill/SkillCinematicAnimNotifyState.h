// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "DataAsset/PrimaryDataAsset/SkillDataAsset/SkillDataAsset.h"
#include "SkillCinematicAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FSRCOPY_API USkillCinematicAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillSetting")
	TObjectPtr<USkillDataAsset> TargetSkill;
};
