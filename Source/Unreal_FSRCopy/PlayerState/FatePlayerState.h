// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "FatePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FSRCOPY_API AFatePlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AFatePlayerState();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resonance")
	TObjectPtr<class UResonanceComponent> ResonanceComponent;
};
