// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/BTService/Servant/UpdateMasterState/UpdateMasterState_BTService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "PlayableCharacter/PlayableBaseCharacter.h"


UUpdateMasterState_BTService::UUpdateMasterState_BTService()
{
	NodeName = TEXT("UpdateMasterState");
	bNotifyTick = true;
}

void UUpdateMasterState_BTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return;

	APlayableBaseCharacter* AIPawn = Cast<APlayableBaseCharacter>(AIController->GetPawn());
	if (!AIPawn) return;
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (BlackboardComp)
	{
		bool bIsCombatMode = AIPawn->GetIsCombatMode();
		BlackboardComp->SetValueAsBool(FName("IsCombatMode"), bIsCombatMode);
		//현재 조종 중인 플레이 타겟 설정
		if (PlayerPawn && PlayerPawn != AIPawn && !bIsCombatMode)
		{
			BlackboardComp->SetValueAsObject(FName("TargetActor"), PlayerPawn);
		}
		else
		{
			BlackboardComp->ClearValue(FName("TargetActor"));
		}
	}
}
