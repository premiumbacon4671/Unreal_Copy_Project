// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LinkSkillButtonUI.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ELinkSkillButtonDirection : uint8
{
	Top UMETA(DisplayName = "Top"),
	Left UMETA(DisplayName = "Left"),
	Right UMETA(DisplayName = "Right"),
	Bottom UMETA(DisplayName = "Bottom")
};

UCLASS()
class UNREAL_FSRCOPY_API ULinkSkillButtonUI : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Button")
	ELinkSkillButtonDirection ButtonDirection{ ELinkSkillButtonDirection::Top };

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UImage> SkillIcon;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UTextBlock> SkillName;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UImage> LinkBall1;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UImage> LinkBall2;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UImage> LinkBall3;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UImage> LinkBallFrame1;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UImage> LinkBallFrame2;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UImage> LinkBallFrame3;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	int32 SkillCost;

	TArray<TObjectPtr<class UImage>> LinkBalls;
	TArray<TObjectPtr<class UImage>> LinkBallFrames;

protected:
	virtual void NativePreConstruct() override;

public:
	void UpdateLinkSkillInfo(UTexture2D* IconTexture, const FString& UpdateSkillName, int32 Cost);
	void SetUpdateLinkBallVisibility(int CurrentLinkCount);
	void SetLinkSkillAvailableState(bool bIsAvailable);
	int32 GetLinkSkillCost() const { return SkillCost; }
};
