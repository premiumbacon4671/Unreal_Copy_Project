// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "UpdateMasterState_BTService.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FSRCOPY_API UUpdateMasterState_BTService : public UBTService
{
	GENERATED_BODY()
	
public:
	UUpdateMasterState_BTService();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
