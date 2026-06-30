// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RecoverItemButton.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FSRCOPY_API URecoverItemButton : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Icon;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UWidgetSwitcher> StyleSwitcher;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ItemName;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ItemCountX;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ItemCount;
	
	UPROPERTY()
	FName ItemID{ NAME_None };
	UPROPERTY()
	FText ItemDescription;
	UPROPERTY()
	TObjectPtr<class URecoverItemMenuUI> OwnerMenu;
	UPROPERTY(EditDefaultsOnly, Category = "UI Style")
	FLinearColor TextColorSelected = FLinearColor::Black;

	UPROPERTY(EditDefaultsOnly, Category = "UI Style")
	FLinearColor TextColorNormal = FLinearColor::White;


	virtual void NativeConstruct() override;
public:
	void InitButtonData(const struct FItemStack& InData, class URecoverItemMenuUI* InOwnerMenu);
	void SetHighlightState(bool bIsHighlighted);
	void UpdateCounterDisplay(int32 NewCount);
	FName GetItemID() const { return ItemID; }
	
};
