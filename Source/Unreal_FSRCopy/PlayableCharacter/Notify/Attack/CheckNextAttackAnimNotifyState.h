// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CheckNextAttackAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FSRCOPY_API UCheckNextAttackAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	bool Received_NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) const;
	bool Received_NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) const;

};
