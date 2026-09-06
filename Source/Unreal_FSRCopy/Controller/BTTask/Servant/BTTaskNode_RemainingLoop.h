// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_RemainingLoop.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FSRCOPY_API UBTTaskNode_RemainingLoop : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTaskNode_RemainingLoop();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
