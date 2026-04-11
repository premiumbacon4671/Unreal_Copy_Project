// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/MonsterAI/BaseMonsterAIController.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"

ABaseMonsterAIController::ABaseMonsterAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeFinder(TEXT("/Script/AIModule.BehaviorTree'/Game/Blueprint/Monster/AI/BT_BaseMonster.BT_BaseMonster'"));
	if (BehaviorTreeFinder.Succeeded())
		BehaviorTreeAsset = BehaviorTreeFinder.Object;
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BlackboardFinder(TEXT("/Script/AIModule.BlackboardData'/Game/Blueprint/Monster/AI/BB_BaseMonster.BB_BaseMonster'"));
	if (BlackboardFinder.Succeeded())
		BlackboardAsset = BlackboardFinder.Object;
}

void ABaseMonsterAIController::PlayBehaviorTree()
{
	UBlackboardComponent* BlackboardComponent = Blackboard.Get();
	if(UseBlackboard(BlackboardAsset, BlackboardComponent))
	{
		ensure(RunBehaviorTree(BehaviorTreeAsset));
	}
}

void ABaseMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	PlayBehaviorTree();
}
