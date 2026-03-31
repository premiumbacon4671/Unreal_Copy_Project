// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseMonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FSRCOPY_API ABaseMonsterAIController : public AAIController
{
	GENERATED_BODY()
private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BlackboardAsset;
	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BehaviorTreeAsset;

public:
	ABaseMonsterAIController();
};
