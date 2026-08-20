// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServantChargeBarUI.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FSRCOPY_API UServantChargeBarUI : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UImage> ServantIcon;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UProgressBar> ServantProgressBar;

public:
	void Init(class UTexture2D* Icon, float ChargePercent);
	void UpdateServantChargeBar(float ChargePercent);

};
