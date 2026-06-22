// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MiyamotoIoriController.generated.h"

/**
 * 
 */
#define CAMERA_SPIN_SPEED 100
struct FInputActionValue;
UCLASS()
class UNREAL_FSRCOPY_API AMiyamotoIoriController : public APlayerController
{
	GENERATED_BODY()
	
private:
	TObjectPtr<class APlayableBaseCharacter> CurPlayableCharacter;
	TObjectPtr<class APlayableBaseCharacter> MiyamotoIori;
	TObjectPtr<class APlayerHUD> PlayerHUD;

#pragma region InputAction
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputAction> MoveAction;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputAction> RotationAction;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputAction> SprintAction;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputAction> EvadeAction;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputAction> JumpAction;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputAction> NormalAttackAction;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputAction> HeavyAttackAction;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputAction> ChangeStanceAction;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputAction> UIMoveAction;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputAction> HikenAction;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputAction> SkillSelectAction;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputAction> SkillHoldAction;

	//hp회복, 아이템 수급 등 임시로 사용할 키
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputAction> ZTestKey;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputMappingContext> MappingContext;
#pragma endregion

	bool isMoveInput{ false };
	bool isCombat{ false };
	bool isUIMode{ false };

public:
	AMiyamotoIoriController();
	void BeginPlay();
	void Tick(float DeltaTime) override;
	void SetupInputComponent() override;
	void MoveInput(const FInputActionValue& value);
	void MoveEndInput(const FInputActionValue& value);
	void LookInput(const FInputActionValue& value);
	void SprintInput(const FInputActionValue& value);
	void EvadeInput(const FInputActionValue& value);
	void JumpInput(const FInputActionValue& value);
	
	void NormalAttackInput(const FInputActionValue& value);
	void HeavyAttackInput(const FInputActionValue& value);
	void HeavyAttackTriggeredInput(const FInputActionValue& value);
	void HeavyAttackCompletedInput(const FInputActionValue& valuve);
	void ChangeStanceInput(const FInputActionValue& value);
	void ChangeStanceCompletedInput(const FInputActionValue& value);
	
	void UIMoveInput(const FInputActionValue& value);

	void HikenInput(const FInputActionValue& value);
	
	void SkillSelectInput(const FInputActionValue& value);
	void SkillHoldTriggeredInput(const FInputActionValue& value);
	void SkillHoldCompletedInput(const FInputActionValue& value);
	bool IsSkillHoldActionPressed() const;

	bool GetIsMoveInput() const { return isMoveInput; }
	UInputAction* GetNormalAttackAction() const { return NormalAttackAction; }
	UInputMappingContext* GetDefaultMappingContext() const { return MappingContext; }

	void ZTestKeyInput(const FInputActionValue& value);
};
