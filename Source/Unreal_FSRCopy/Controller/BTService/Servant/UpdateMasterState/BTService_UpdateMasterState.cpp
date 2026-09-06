// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/BTService/Servant/UpdateMasterState/BTService_UpdateMasterState.h"
#include "PlayableCharacter/PlayableBaseCharacter.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTService_UpdateMasterState::UBTService_UpdateMasterState()
{
	NodeName = TEXT("UpdateMasterStateV2");
	bNotifyTick = true;
}

void UBTService_UpdateMasterState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
		//비전투 상황에서만
		if(!bIsCombatMode)
		{
			//현재 조종 중인 플레이 타겟 설정
			if (PlayerPawn && PlayerPawn != AIPawn)
			{
				BlackboardComp->SetValueAsObject(FName("TargetActor"), PlayerPawn);
				if (ACharacter* PlayerCharacter = Cast<ACharacter>(PlayerPawn))
				{
					float MasterSpeed = PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed;
					AIPawn->GetCharacterMovement()->MaxWalkSpeed = MasterSpeed;
				}
			}
		}
	}
}
