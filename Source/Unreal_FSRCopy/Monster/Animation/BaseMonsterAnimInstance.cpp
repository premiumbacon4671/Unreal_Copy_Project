// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Animation/BaseMonsterAnimInstance.h"
#include "BaseMonsterAnimInstance.h"
#include "GameFramework/Character.h"
#include "Monster/BaseMonster.h"

void UBaseMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if(ABaseMonster* OwnerCharacter = Cast<ABaseMonster>(TryGetPawnOwner()))
	{
		MoveSpeed = OwnerCharacter->GetVelocity().Size();
		IsMove = MoveSpeed > 0.0f;
		IsDead = OwnerCharacter->IsDead();
		if (IsDead)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("IsDead : ") + OwnerCharacter->GetName());
		}
	}
}
