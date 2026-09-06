// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_StrafeMode.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FSRCOPY_API UBTService_StrafeMode : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_StrafeMode();

protected:
	void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
