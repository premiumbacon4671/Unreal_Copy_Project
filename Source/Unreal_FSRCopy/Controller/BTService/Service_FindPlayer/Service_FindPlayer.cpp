// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/BTService/Service_FindPlayer/Service_FindPlayer.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"


UService_FindPlayer::UService_FindPlayer()
{
	Interval = 1.0f;
	bNotifyTick = true;
	NodeName = TEXT("FindPlayer");
}

void UService_FindPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	APawn* OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();
	FHitResult HitResult;
	bool isHit = UKismetSystemLibrary::BoxTraceSingle(
		this,
		OwnerPawn->GetActorLocation() + OwnerPawn->GetActorForwardVector() * 100.0f, // Start
		OwnerPawn->GetActorLocation() + OwnerPawn->GetActorForwardVector() * 500.0f, // End
		FVector(200.0f, 200.0f, 200.0f), // Box Extent
		OwnerPawn->GetActorRotation(), // Orientation
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel5), // Trace Channel
		false, // bTraceComplex
		{ OwnerPawn }, // Actors to Ignore
		EDrawDebugTrace::ForDuration, // Draw Debug Type
		HitResult, // Out Hit
		true // bIgnoreSelf
	);
	if (isHit)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("ObjectTarget"), HitResult.GetActor());
		DrawDebugLine(OwnerPawn->GetWorld(), OwnerPawn->GetActorLocation(), HitResult.GetActor()->GetActorLocation(), FColor::Red, false, 5.0f);
		DrawDebugSphere(OwnerPawn->GetWorld(), HitResult.GetActor()->GetActorLocation(), 50.0f, 12, FColor::Red, false, 5.0f);
	}
}
