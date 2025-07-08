// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MiyamotoIoriController.generated.h"

/**
 * 
 */
struct FInputActionValue;
UCLASS()
class UNREAL_FSRCOPY_API AMiyamotoIoriController : public APlayerController
{
	GENERATED_BODY()
	
private:
	TObjectPtr<class APlayableBaseCharacter> CurPlayableCharacter;
	TObjectPtr<class APlayableBaseCharacter> MiyamotoIori;

#pragma region InputAction
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputAction> MoveAction;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputAction> RotationAction;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInputMappingContext> MappingContext;

	bool isMoveInput{ false };
#pragma endregion

public:
	AMiyamotoIoriController();
	void BeginPlay();
	void Tick(float DeltaTime);
	void SetupInputComonent() override;
	void MoveInput(const FInputActionValue& value);
	void MoveEndInput(const FInputActionValue& value);
	void LookInput(const FInputActionValue& value);

	bool GetIsMoveInput() const { return isMoveInput; }
};
