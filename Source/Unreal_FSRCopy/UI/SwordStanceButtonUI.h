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
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UImage> SwordStance;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Frame;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UImage> Highlight;
	
public:
	void SetColor(FString HexColor);
	void SetSwordStanceVisibility(ESlateVisibility eVisibility);
	void SetHighlightVisibility(ESlateVisibility eVisibility);
};
