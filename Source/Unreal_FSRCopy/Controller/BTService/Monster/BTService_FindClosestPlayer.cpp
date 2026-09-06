// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/BTService/Monster/BTService_FindClosestPlayer.h"

#include "PlayableCharacter/PlayableBaseCharacter.h"
#include "ActorComponent/StateComponent/PlayableStateComponent.h"
#include "Monster/BaseMonster.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "EngineUtils.h"

UBTService_FindClosestPlayer::UBTService_FindClosestPlayer()
{
	NodeName = TEXT("Find Closest Player");
	bNotifyTick = true;
}

void UBTService_FindClosestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	AAIController* AICon = OwnerComp.GetAIOwner();
	if(!BB || !AICon)
		return;
	APawn* AIPawn = AICon->GetPawn();
	if(!AIPawn)
		return;
	AActor* ExistingTarget = Cast<AActor>(BB->GetValueAsObject(FName("TargetActor")));
	APlayableBaseCharacter* ExistingPlayer = Cast<APlayableBaseCharacter>(ExistingTarget);
	
	//타	겟이 있으면 유지
	if (ExistingPlayer && !ExistingPlayer->GetStatusComponent()->IsDead())
	{
		return;
	}

	APlayableBaseCharacter* BestTarget = nullptr;
	float ClosestDistSqr = FLT_MAX;
	FVector AILoc = AIPawn->GetActorLocation();
	for(TActorIterator<APlayableBaseCharacter> iter(GetWorld()); iter; ++iter)
	{
		APlayableBaseCharacter* Player = *iter;
		if(!Player || Player->GetStatusComponent()->IsDead())
			continue;
		float DistSqr = FVector::DistSquared(AILoc, Player->GetActorLocation());
		if(DistSqr < ClosestDistSqr)
		{
			ClosestDistSqr = DistSqr;
			BestTarget = Player;
		}
	}

	if(BestTarget)
	{
		BB->SetValueAsObject(FName("TargetActor"), BestTarget);
		AICon->SetFocus(BestTarget);
		if(ABaseMonster* Monster = Cast<ABaseMonster>(AIPawn))
		{
			Monster->SetCurrentTarget(BestTarget);
		}
	}
	else
	{
		BB->ClearValue(FName("TargetActor"));
		AICon->ClearFocus(EAIFocusPriority::Gameplay);
		if(ABaseMonster* Monster = Cast<ABaseMonster>(AIPawn))
		{
			Monster->ClearLockOnTargetAI();
		}
	}
}
