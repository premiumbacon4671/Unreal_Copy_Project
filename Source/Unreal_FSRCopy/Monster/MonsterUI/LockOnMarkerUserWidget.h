// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LockOnMarkerUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FSRCOPY_API ULockOnMarkerUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> LockOnMarker;

};
