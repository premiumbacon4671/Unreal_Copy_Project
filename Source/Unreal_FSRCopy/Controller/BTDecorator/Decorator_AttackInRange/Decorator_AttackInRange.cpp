// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/BTDecorator/Decorator_AttackInRange/Decorator_AttackInRange.h"
#include "Monster/BaseMonster.h"
#include "PublicUse/AttackCombatStruct/AttackCombatStruct.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UDecorator_AttackInRange::UDecorator_AttackInRange()
{
	NodeName = TEXT("Attack In Range");
	bNotifyTick = true;
}

bool UDecorator_AttackInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	AAIController* AICon = OwnerComp.GetAIOwner();
	if(!BB || !AICon)
		return false;
	ABaseMonster* MonsterPawn = Cast<ABaseMonster>(AICon->GetPawn());
	AActor* TargetActor = Cast<AActor>(BB->GetValueAsObject(FName("TargetActor")));

	if(!MonsterPawn || !TargetActor)
		return false;
	FAttackData AttackData = MonsterPawn->GetMonsterAttackData();

	float AttackRange = AttackData.AttackTraceData.ForwardDistance * 2;
	float Distance = FVector::Dist(MonsterPawn->GetActorLocation(), TargetActor->GetActorLocation());
	return Distance <= AttackRange;
}
