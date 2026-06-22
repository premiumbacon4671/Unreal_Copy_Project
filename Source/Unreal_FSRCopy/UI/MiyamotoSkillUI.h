// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MiyamotoSkillUI.generated.h"

/**
 * 
 */
class UMiyamotoSkillButtonUI;

UCLASS()
class UNREAL_FSRCOPY_API UMiyamotoSkillUI : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UMiyamotoSkillButtonUI> TopSkillButton;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UMiyamotoSkillButtonUI> LeftSkillButton;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UMiyamotoSkillButtonUI> RightSkillButton;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UMiyamotoSkillButtonUI> BottomSkillButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UTextBlock> GemCountText;
public:
	void NativeConstruct() override;
	void UpdateAllSkillsAvailability(int32 CurrentGemCount);
	void Init(class AMiyamoto_Iori* MiyamotoIori);

	void StartedSkillUI();
	void EndedSkillUI();
	UFUNCTION()
	void UpdateGemCount(FName ItemID, int32 CurrentGemCount);
};
