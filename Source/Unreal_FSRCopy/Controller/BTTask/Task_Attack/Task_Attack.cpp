// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/BTTask/Task_Attack/Task_Attack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Monster/BaseMonster.h"
#include "PlayableCharacter/PlayableBaseCharacter.h"

UTask_Attack::UTask_Attack()
{

}

EBTNodeResult::Type UTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	ABaseMonster* Owner = Cast<ABaseMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if(nullptr == Owner)
		return EBTNodeResult::Failed;

	APlayableBaseCharacter* Target = Cast<APlayableBaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("ObjectTarget"))));
	if(nullptr == Target)
		return EBTNodeResult::Failed;

	Owner->OnAttackMontageEndedDelegate.BindLambda([&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		});

	Owner->MonsterNormalAttack(Target);
	//작업이 진행중임을 나타내며, 작업이 완료되면 FinishLatentTask 함수를 호출하여 작업이 성공적으로 완료되었음을 알립니다.
	return EBTNodeResult::InProgress;
}
