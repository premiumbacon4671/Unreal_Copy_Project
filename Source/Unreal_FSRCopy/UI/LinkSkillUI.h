// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interface/UIInputReceiverInterface.h"
#include "LinkSkillUI.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FSRCOPY_API ULinkSkillUI : public UUserWidget, public IUIInputReceiverInterface
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class ULinkSkillButtonUI> TopLinkSkillButton;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class ULinkSkillButtonUI> LeftLinkSkillButton;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class ULinkSkillButtonUI> RightLinkSkillButton;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class ULinkSkillButtonUI> BottomLinkSkillButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UProgressBar> LinkProgressBar;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UImage> LinkBall1;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UImage> LinkBall2;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UImage> LinkBall3;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UImage> LinkBall4;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UImage> LinkBall5;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UImage> LinkBall6;

	TArray<TObjectPtr<class UImage>> LinkBalls;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UServantChargeBarUI> ServantChargeBar1;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UServantChargeBarUI> ServantChargeBar2;
public:
	void NativeConstruct() override;
	void UpdateAllLinkSkillsAvailability(int32 CurrentLinkCount, float CurrentLinkProgress);
	void Init(class AServantBaseCharacter* Servant);

	virtual void OnInterfaceOpen() override;
	virtual void OnInterfaceClose() override;
	virtual void OnInterfaceConfirm() override;
	virtual void OnInterfaceMove(FIntPoint MoveDirection) override;

	void RefreshPartyUI(class AServantBaseCharacter* Seervant);
};
