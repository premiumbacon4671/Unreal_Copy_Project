// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayableStatusUI.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FSRCOPY_API UPlayableStatusUI : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> HPProgressBar;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> HikenProgressBar;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> LinkProgressBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> LinkBall1;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> LinkBall2;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> LinkBall3;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> LinkBall4;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> LinkBall5;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> LinkBall6;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UEarthStanceShieldUI> EarthStanceShieldBar;
	TArray<TObjectPtr<class UImage>> LinkBalls;

	UPROPERTY()
	TObjectPtr<class UPlayableStateComponent> CurrentStateComponent;

public:
	void NativeConstruct() override;


	void SetLinkBallVisibility(int Index, ESlateVisibility InVisibility);
	UFUNCTION()
	void Init(class APlayableBaseCharacter* Character);
	void SetHPBarPercent(float Percent);
	void SetHikenBarPercent(float Percent);
	void SetLinkBarPercent(float Percent);

	UFUNCTION()
	void HandleUpdateHp(float Percent);
	UFUNCTION()
	void HandleUpdateHiken(float Percent);
	UFUNCTION()
	void HandleUpdateLinkSkillGauge(float Percent);
	UFUNCTION()
	void HandleUpdateLinkSkillBall(int Count);

	void SwitchTargetStatusComponent(class UPlayableStateComponent* NewStatusComponent);
};
