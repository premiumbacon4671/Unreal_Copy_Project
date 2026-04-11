// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "Service_FindPlayer.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FSRCOPY_API UService_FindPlayer : public UBTService
{
	GENERATED_BODY()
public:
	UService_FindPlayer();
protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
