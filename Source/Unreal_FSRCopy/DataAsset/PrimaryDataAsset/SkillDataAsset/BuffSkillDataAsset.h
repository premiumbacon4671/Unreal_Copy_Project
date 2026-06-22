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
	//버프 지속 시간
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill | Buff")
	float BuffDuration{ 10.0f };

	//능력 상승 비율
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill | Buff")
	float BuffMultiplier{ 0.5f };
};
