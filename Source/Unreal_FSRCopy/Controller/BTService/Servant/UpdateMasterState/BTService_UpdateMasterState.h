// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_UpdateMasterState.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FSRCOPY_API UBTService_UpdateMasterState : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_UpdateMasterState();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
