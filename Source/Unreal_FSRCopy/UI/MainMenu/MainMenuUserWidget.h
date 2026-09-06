// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interface/UIInputReceiverInterface.h"
#include "MainMenuUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FSRCOPY_API UMainMenuUserWidget : public UUserWidget, public IUIInputReceiverInterface
{
	GENERATED_BODY()
	
protected:
	int32 CurrentSelectedIndex = 0;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UMainMenuButtonUserWidget> GameStartButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UMainMenuButtonUserWidget> ExitButton;

	TArray<TObjectPtr<class UMainMenuButtonUserWidget>> ButtonArray;

public:
	virtual void NativeConstruct() override;
	virtual void OnInterfaceOpen() override;
	virtual void OnInterfaceClose() override;
	virtual void OnInterfaceConfirm() override;
	virtual void OnInterfaceCancel() override;
	virtual void OnInterfaceMove(FIntPoint MoveDirection) override;

	UFUNCTION()
	void OnStartGameClicked();
	UFUNCTION()
	void OnExitGameClicked();
};
