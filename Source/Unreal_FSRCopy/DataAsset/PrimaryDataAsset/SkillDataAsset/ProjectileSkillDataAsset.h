// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/PrimaryDataAsset/SkillDataAsset/SkillDataAsset.h"
#include "ProjectileSkillDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FSRCOPY_API UProjectileSkillDataAsset : public USkillDataAsset
{
	GENERATED_BODY()
public :
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill | Projectile")
	TSubclassOf<class AFateProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill | Projectile")
	float ProjectileSpeed{ 1000.0f };
};
