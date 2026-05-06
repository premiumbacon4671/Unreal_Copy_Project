// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"

#include "PublicUse/AttackCombatStruct/AttackCombatStruct.h"
#include "AttackTraceAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FSRCOPY_API UAttackTraceAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	EAttackVariety AttackVariety;

public:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
