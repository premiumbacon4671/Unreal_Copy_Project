// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/BTDecorator/Decorator_AttackInRange/Decorator_AttackInRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UDecorator_AttackInRange::UDecorator_AttackInRange()
{
	NodeName = TEXT("Attack In Range");
}

bool UDecorator_AttackInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	APawn* OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == OwnerPawn)
		return false;
	APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("ObjectTarget")));
	if (nullptr == TargetPawn)
		return false;
	float Distance = OwnerPawn->GetDistanceTo(TargetPawn);
	if (Distance <= 300.0f)
		return true;
    return false;
}
