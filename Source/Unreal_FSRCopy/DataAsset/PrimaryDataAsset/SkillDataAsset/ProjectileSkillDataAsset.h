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
	UPROPERTY(EditAnywhere, Category = "Skill | Projectile")
	TSubclassOf<class AFateProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Skill | Projectile")
	float ProjectileSpeed{ 1000.0f };

	UPROPERTY(EditAnywhere, Category = "Skill | Explosion")
	float ExplosionRadius{ 200.0f };

	UPROPERTY(EditAnywhere, Category = "Skill | Distance")
	float MaxTravelDistance{ 1500.0f };
};
