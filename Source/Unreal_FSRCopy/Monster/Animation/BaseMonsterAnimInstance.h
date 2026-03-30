// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseMonsterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FSRCOPY_API UBaseMonsterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimState", meta = (AllowPrivateAccess = "true"))
	float MoveSpeed{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimState", meta = (AllowPrivateAccess = "true"))
	bool IsMove{ false };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimState", meta = (AllowPrivateAccess = "true"))
	bool IsDead{ false };
public:
	void NativeUpdateAnimation(float DeltaSeconds) override;
};
