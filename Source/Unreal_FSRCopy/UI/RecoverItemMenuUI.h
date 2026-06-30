// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/PlayableBaseUserWidget.h"
#include "UI/RecoverItemButton.h"
#include "Interface/UIInputReceiverInterface.h"
#include "RecoverItemMenuUI.generated.h"

/**
 * 
 */

UCLASS()
class UNREAL_FSRCOPY_API URecoverItemMenuUI : public UUserWidget, public IUIInputReceiverInterface
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UScrollBox> ItemMenu;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ItemDescription;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<URecoverItemButton> ItemButtonClass;

	UPROPERTY()
	TObjectPtr<class APlayableBaseCharacter> CurPlayableCharacter;
	//생성된 버튼을 관리하는 배열(포커스, 선택)
	UPROPERTY()
	TArray <TObjectPtr<URecoverItemButton>> CreatedButtons;
	int32 CurrentSelectedIndex = 0;
public:
	URecoverItemMenuUI(const FObjectInitializer& ObjectInitializer);
	void RefreshItemList(const TArray<struct FItemStack>& InItemList);
	void UpdateDescription(const FText& NewDescription);
	virtual void OnInterfaceOpen() override;
	virtual void OnInterfaceClose() override;
	virtual void OnInterfaceMove(FIntPoint MoveDirection) override;
	virtual void OnInterfaceConfirm() override;
	virtual void OnInterfaceCancel() override;
};
