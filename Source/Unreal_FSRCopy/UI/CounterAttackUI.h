// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InputCoreTypes.h"
#include "CounterAttackUI.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FSRCOPY_API UCounterAttackUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> KeyInputImage;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> CounterAttackText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "InuputData")
	TMap<FKey, TObjectPtr<class UTexture2D>> KeyInputIconeMap;

public:
	void UpdateKeyIcon(FKey Key);
};
