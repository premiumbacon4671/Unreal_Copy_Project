// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MiyamotoSkillButtonUI.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ESkillButtonDirection : uint8
{
	Top UMETA(DisplayName = "Top"),
	Left UMETA(DisplayName = "Left"),
	Right UMETA(DisplayName = "Right"),
	Bottom UMETA(DisplayName = "Bottom")
};

UCLASS()
class UNREAL_FSRCOPY_API UMiyamotoSkillButtonUI : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Button")
	ESkillButtonDirection ButtonDirection{ESkillButtonDirection::Top};


private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UImage> SkillIcon;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UTextBlock> SkillName;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UTextBlock> SkillCostText;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	int32 SkillCost;

protected:
	virtual void NativePreConstruct() override;

public:
	void UpdateSkillInfo(UTexture2D* IconTexture, const FString& UpdateSkillName, int32 Cost);
	void SetSkillAvailableState(bool bIsAvailable);
	int32 GetSkillCost() const { return SkillCost; }
};
