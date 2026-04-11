// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/BTTask/Task_SetRandomTargetPoint/Task_SetRandomTargetPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "NavigationSystem.h"
#include "AIController.h"

UTask_SetRandomTargetPoint::UTask_SetRandomTargetPoint()
{
}

EBTNodeResult::Type UTask_SetRandomTargetPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	APawn* Owner = OwnerComp.GetAIOwner()->GetPawn();
	if(nullptr == Owner)
		return EBTNodeResult::Type::Failed;
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(Owner);
	if(nullptr == NavSystem)
		return EBTNodeResult::Type::Failed;
	FNavLocation RandomLocation;
	if(NavSystem->GetRandomPointInNavigableRadius(Owner->GetActorLocation(), 400.f, RandomLocation))
	{
		UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
		if(BlackboardComponent)	
		{
			BlackboardComponent->SetValueAsVector(TEXT("MoveTarget"), RandomLocation.Location);
			DrawDebugSphere(GetWorld(), RandomLocation.Location, 50.f, 12, FColor::Red, false, 20.f);
		}
		else
			return EBTNodeResult::Type::Failed;
	}
	else
		return EBTNodeResult::Type::Failed;
	return EBTNodeResult::Type::Succeeded;
}
