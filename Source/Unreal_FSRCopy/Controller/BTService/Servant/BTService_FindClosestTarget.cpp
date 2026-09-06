// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/BTService/Servant/BTService_FindClosestTarget.h"
#include "PlayableCharacter/PlayableBaseCharacter.h"
#include "Monster/BaseMonster.h"
#include "CombatZone/CombatZone.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTService_FindClosestTarget::UBTService_FindClosestTarget()
{
	NodeName = TEXT("Finde Closset Target");
	bNotifyTick = true;
}

void UBTService_FindClosestTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if(!BB)
		return;
	AAIController* AIController = OwnerComp.GetAIOwner();
	if(!AIController)
		return;

	APlayableBaseCharacter* AIPawn = Cast<APlayableBaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if(!AIPawn || !AIPawn->GetCurrentCombatZone())
		return;

	AActor* ExistingTargetActor = Cast<AActor>(BB->GetValueAsObject(FName("TargetActor")));
	ABaseMonster* ExistingMonster = Cast<ABaseMonster>(ExistingTargetActor);

	//이미 타겟이 된 몬스터가 살아있으면 그대로 유지
	if (ExistingMonster && !ExistingMonster->IsDead())
	{
		return;
	}

	TArray<ABaseMonster*> LiveMonsters = AIPawn->GetCurrentCombatZone()->GetLiveMonsters();
	LiveMonsters.RemoveAll([](ABaseMonster* Monster) {
		return nullptr == Monster || Monster->IsDead();
		});

	if(LiveMonsters.Num() == 0)
	{
		OwnerComp.GetBlackboardComponent()->ClearValue(TEXT("TargetActor"));
		AIPawn->ClearCurrentTarget();
		AIController->ClearFocus(EAIFocusPriority::Gameplay);
		return;
	}

	FVector AILoc = AIPawn->GetActorLocation();
	Algo::Sort(LiveMonsters, [AILoc](ABaseMonster* A, ABaseMonster* B)
		{
			return FVector::DistSquared(AILoc, A->GetActorLocation()) < FVector::DistSquared(AILoc, B->GetActorLocation());
		});

	ABaseMonster* BestTarget = LiveMonsters[0];
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName("TargetActor"), BestTarget);

	AIPawn->SetCurrentTarget(BestTarget);
	AIController->SetFocus(BestTarget);
}
