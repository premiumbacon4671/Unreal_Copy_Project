// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayableBaseUserWidget.generated.h"

/**
 * 
 */
//HideAndKeepState HUD와 같은 포커스의 상태 유지를 해야 될 경우 사용
//DestoryAndReset 옵션 창과 같이 포커스를 초기화 될 경우 사용
UENUM(BlueprintType)
enum class EUICloseBehavior : uint8
{
	HideAndKeepState UMETA(DisplayName = "상태유지 (Hidden)"),
	DestoryAndReset UMETA(DisplayName = "완전 파괴 및 초기화 (RemoveFromParent")
};

//리스트 탐색과 같은 포커스 기능을 통해 선택 할 수 있는 UI를 위한 BaseClass
UCLASS()
class UNREAL_FSRCOPY_API UPlayableBaseUserWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI Settings")
	EUICloseBehavior UICloseBehavior{ EUICloseBehavior::HideAndKeepState };

	virtual void NativeConstruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

public:
	virtual bool HandleBackAction();
	EUICloseBehavior GetUICloseBehavior() { return UICloseBehavior; }
};
