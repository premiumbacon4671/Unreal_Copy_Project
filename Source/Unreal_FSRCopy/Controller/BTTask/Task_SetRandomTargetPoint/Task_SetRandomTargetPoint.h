// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Task_SetRandomTargetPoint.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FSRCOPY_API UTask_SetRandomTargetPoint : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UTask_SetRandomTargetPoint();
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
