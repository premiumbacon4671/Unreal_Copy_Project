// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/BTService/Servant/BTService_StrafeMode.h"

#include "PlayableCharacter/PlayableBaseCharacter.h"
#include "Monster/BaseMonster.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTService_StrafeMode::UBTService_StrafeMode()
{
	NodeName = TEXT("Strafe Mode");
	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = true;
}

void UBTService_StrafeMode::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	if (APlayableBaseCharacter* AIPawn = Cast<APlayableBaseCharacter>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		AIPawn->SetStrafeMovementMode(true);
	}
	else if (ABaseMonster* MonsterPaw = Cast<ABaseMonster>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		MonsterPaw->SetStrafeMovementMode(true);
	}
}

void UBTService_StrafeMode::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);
	if (APlayableBaseCharacter* AIPawn = Cast<APlayableBaseCharacter>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		AIPawn->SetStrafeMovementMode(false);
	}
	else if (ABaseMonster* MonsterPaw = Cast<ABaseMonster>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		MonsterPaw->SetStrafeMovementMode(false);
	}
}
