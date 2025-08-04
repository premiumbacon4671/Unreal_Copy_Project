// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SwordStanceButtonUI.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FSRCOPY_API USwordStanceButtonUI : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> SwordStance;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Frame;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Highlight;

	void SetColor(FLinearColor color);
};
