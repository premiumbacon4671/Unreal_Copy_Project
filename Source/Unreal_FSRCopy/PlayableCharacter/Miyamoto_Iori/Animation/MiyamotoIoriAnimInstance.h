// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MiyamotoIoriAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FSRCOPY_API UMiyamotoIoriAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class APlayableBaseCharacter> OwnerCharacter;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCharacterMovementComponent> MovementComponent;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool isMove;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float MoveVelocity;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool isRun;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool isJump;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool isMoveInput;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool isEquip;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int CurSwordStance;
public:
	void NativeInitializeAnimation() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "MiyamotoIori")
	UAnimSequence* GetCombatIdleSequence() const;
};
