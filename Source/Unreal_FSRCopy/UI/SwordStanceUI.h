// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SwordStanceUI.generated.h"

/**
 * 
 */

enum class ESWORDSTANCE : uint8;

UCLASS()
class UNREAL_FSRCOPY_API USwordStanceUI : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> EarthStance;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Earth_Frame;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Earth_Highlight;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> WaterStance;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Water_Frame;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Water_Highlight;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> FireStance;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Fire_Frame;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Fire_Highlight;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> WindStance;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Wind_Frame;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Wind_Highlight;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> VoidStance;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Void_Frame;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Void_Highlight;

public:
	void Init(ESWORDSTANCE CurSwordStance, TMap<ESWORDSTANCE, bool> isUnlockSwordStance);
};
