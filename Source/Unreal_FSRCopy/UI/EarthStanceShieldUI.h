// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EarthStanceShieldUI.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FSRCOPY_API UEarthStanceShieldUI : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> ShieldProgressBar;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class UEarthStanceActorComponent> EarthStanceActorComponent;
public:
	virtual void NativeConstruct() override;
	UFUNCTION()
	void HandleShieldChanged(float ShieldPercent);
	UFUNCTION()
	void HandleStanceStateChanged(bool bIsActive);
	void InitShieldUI(class UEarthStanceActorComponent* InEarthStanceActorComponent);
};
