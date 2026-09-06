// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interface/UIInputReceiverInterface.h"
#include "GameMenuUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FSRCOPY_API UGameMenuUserWidget : public UUserWidget, public IUIInputReceiverInterface
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UMainMenuButtonUserWidget> GoMainMenu;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UMainMenuButtonUserWidget> ExitGame;

	TArray<TObjectPtr<class UMainMenuButtonUserWidget>> ButtonArray;

	int32 CurrentSelectedIndex = 0;
public:
	virtual void NativeConstruct() override;
	virtual void OnInterfaceOpen() override;
	virtual void OnInterfaceClose() override;
	virtual void OnInterfaceConfirm() override;
	virtual void OnInterfaceCancel() override;
	virtual void OnInterfaceMove(FIntPoint MoveDirection) override;

	UFUNCTION()
	void OnGoMainMenuClicked();
	UFUNCTION()
	void OnExitGameClicked();
};
