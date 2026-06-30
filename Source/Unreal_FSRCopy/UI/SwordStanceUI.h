// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interface/UIInputReceiverInterface.h"
#include "SwordStanceUI.generated.h"

/**
 * 
 */

enum class ESWORDSTANCE : uint8;

UCLASS()
class UNREAL_FSRCOPY_API USwordStanceUI : public UUserWidget, public IUIInputReceiverInterface
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class USwordStanceButtonUI> SwordButtonClass;
	//3X3버튼 UI를 위한 변수
	TArray<TArray<TObjectPtr<class USwordStanceButtonUI>>> Buttons;
	//bindwidget사용하기 위한 변수
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class USwordStanceButtonUI> EarthStance;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class USwordStanceButtonUI> WaterStance;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class USwordStanceButtonUI> FireStance;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class USwordStanceButtonUI> WindStance;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class USwordStanceButtonUI> VoidStance;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCanvasPanel> CanvasPanel;

	TObjectPtr<class AMiyamoto_Iori> CurPlayableCharacter;

	//X = row Y = Column
	FIntPoint CurStanceIndex;
	FIntPoint ChangeStanceIndex;
public:
	void Init(class AMiyamoto_Iori* Miyamoto);
	void NativeConstruct() override;
	bool Initialize() override;


	//void StartedSwordStanceUI();
	//void EndedSwordStanceUI(class AMiyamoto_Iori* Miyamoto);

	virtual void OnInterfaceOpen() override;
	virtual void OnInterfaceClose() override;

	//void SelectSwordStance(FIntPoint StanceIndex);
	virtual void OnInterfaceMove(FIntPoint MoveDirection) override;
};
