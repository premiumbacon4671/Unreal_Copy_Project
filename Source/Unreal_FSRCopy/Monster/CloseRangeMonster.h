// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/BaseMonster.h"
#include "CloseRangeMonster.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FSRCOPY_API ACloseRangeMonster : public ABaseMonster
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float MeleeAttackRange = 150.0f;

public:
	ACloseRangeMonster();
	virtual void AttackTrace(EAttackVariety AttackVariety) override;
};
