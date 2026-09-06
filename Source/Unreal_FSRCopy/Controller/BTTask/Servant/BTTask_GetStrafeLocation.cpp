// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/BTTask/Servant/BTTask_GetStrafeLocation.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Actor.h"

UBTTask_GetStrafeLocation::UBTTask_GetStrafeLocation()
{
	NodeName = TEXT("Get Strafe Location");
}

EBTNodeResult::Type UBTTask_GetStrafeLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if(!TargetActor || !AIPawn)
	{
		return EBTNodeResult::Failed;
	}
	FVector TargetToAI = AIPawn->GetActorLocation() - TargetActor->GetActorLocation();
	TargetToAI.Z = 0.0f;
	TargetToAI.Normalize();

	float FixedDistance = 200.0f;
	float FixedAngle = 35.0f;

	bool bMoveRight = FMath::RandBool();
	float AngleToApply = bMoveRight ? FixedAngle : -FixedAngle;
	FVector NextStrafeDir = TargetToAI.RotateAngleAxis(AngleToApply, FVector::UpVector);
	FVector StrafeLocation = TargetActor->GetActorLocation() + (NextStrafeDir * FixedDistance);
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName("StrafeLocation"), StrafeLocation);

	return EBTNodeResult::Succeeded;
}
