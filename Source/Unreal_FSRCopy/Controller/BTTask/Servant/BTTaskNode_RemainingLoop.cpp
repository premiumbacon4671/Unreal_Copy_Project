// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/BTTask/Servant/BTTaskNode_RemainingLoop.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTTaskNode_RemainingLoop::UBTTaskNode_RemainingLoop()
{
	NodeName = TEXT("Remaining Loop");
}

EBTNodeResult::Type UBTTaskNode_RemainingLoop::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon) return EBTNodeResult::Failed;

	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (!BBComp) return EBTNodeResult::Failed;
	int32 RandomLoopCount = FMath::RandRange(1, 3); // 원하는 최소~최대 횟수
	BBComp->SetValueAsInt(FName("RemainingLoops"), RandomLoopCount);
	
	return EBTNodeResult::Succeeded;
}
