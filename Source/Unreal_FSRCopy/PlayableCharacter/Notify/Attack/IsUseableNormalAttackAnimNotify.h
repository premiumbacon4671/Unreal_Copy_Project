// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "IsUseableNormalAttackAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FSRCOPY_API UIsUseableNormalAttackAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};
