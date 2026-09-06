// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuButtonUserWidget.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCustomMainMenuButtonClicked);

UCLASS()
class UNREAL_FSRCOPY_API UMainMenuButtonUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UImage> FocusCover;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UTextBlock> ButtonText;

public:
	virtual void NativeOnInitialized() override;
	void InitializeButton(FName InButtonText);
	void SetButtonFocusState(bool bIsFocused);

	//UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCustomMainMenuButtonClicked OnCustomMainMenuButtonClicked;

	UFUNCTION(BlueprintCallable, Category = "MainMenuButton")
	void ConfirmButtonClick();
};
