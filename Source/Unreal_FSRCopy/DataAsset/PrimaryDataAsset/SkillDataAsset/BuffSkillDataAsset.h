// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/PrimaryDataAsset/SkillDataAsset/SkillDataAsset.h"
#include "BuffSkillDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FSRCOPY_API UBuffSkillDataAsset : public USkillDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill | Buff")
	float BuffDuration{ 10.0f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill | Buff")
	float CommonAttackSlowModifier{ 0.5f };
};
