// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/BTTask/Servant/BTTask_PerformNormalAttack.h"
#include "PlayableCharacter/PlayableBaseCharacter.h"
#include "PlayableCharacter/Miyamoto_Iori/ActorComponent/BaseSwordStanceActorComponent.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTTask_PerformNormalAttack::UBTTask_PerformNormalAttack()
{
	NodeName = TEXT("Perform Normal Attack");
}

EBTNodeResult::Type UBTTask_PerformNormalAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APlayableBaseCharacter* AIPawn = Cast<APlayableBaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!AIPawn)
		return EBTNodeResult::Failed;
	int32 AttackIndex = FMath::RandRange(0, 1);
	AIPawn->SnapToTargetEnemy();
	float Duration = AIPawn->GetCurSwordStanceComponent()->ExcuteAINormalAttack(AttackIndex);
	if(Duration <= 0.f)
		return EBTNodeResult::Failed;
	UAnimInstance* AnimInstance = AIPawn->GetMesh() ? AIPawn->GetMesh()->GetAnimInstance() : nullptr;
	UAnimMontage* AttackMontage = AIPawn->GetCurSwordStanceComponent()->GetNormalAttackMontage();
	if (AnimInstance && AttackMontage)
	{
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &UBTTask_PerformNormalAttack::OnAttackMontageEnded, &OwnerComp);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackMontage);
	}

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_PerformNormalAttack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController && AIController->GetPawn())
	{
		APlayableBaseCharacter* AIPawn = Cast<APlayableBaseCharacter>(AIController->GetPawn());
		if (AIPawn)
		{
			AIPawn->StopAnimMontage();
		}
	}

	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UBTTask_PerformNormalAttack::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted, UBehaviorTreeComponent* OwnerComp)
{
	if (OwnerComp)
	{
		EBTNodeResult::Type Result = bInterrupted ? EBTNodeResult::Failed : EBTNodeResult::Succeeded;
		FinishLatentTask(*OwnerComp, Result);
	}
}
