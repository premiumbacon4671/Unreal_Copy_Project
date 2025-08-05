// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
enum class ESWORDSTANCE : uint8;
UCLASS()
class UNREAL_FSRCOPY_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
	
private:
	TSubclassOf<class USwordStanceUI> SwordStanceWidget;
	TObjectPtr<class USwordStanceUI> SwordStanceUI;
public:
	APlayerHUD();
	void BeginPlay() override;
	void PostInitializeComponents() override;
	void SetSwordStanceUIVisibility(ESlateVisibility eVisibility);
};
