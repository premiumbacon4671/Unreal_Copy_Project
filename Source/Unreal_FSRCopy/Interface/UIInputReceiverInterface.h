// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Math/IntPoint.h"
#include "UIInputReceiverInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUIInputReceiverInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNREAL_FSRCOPY_API IUIInputReceiverInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//UI가 함수를 사용하지 않을 수도 있기에 가상함수로만 제작
	// WASD로 이동을 관리하는 UI만 사용
	//UI 열림
	virtual void OnInterfaceOpen() {}
	//UI 닫힘
	virtual void OnInterfaceClose() {}
	//UI 확인
	virtual void OnInterfaceConfirm() {}
	//UI 취소
	virtual void OnInterfaceCancel() {}
	//UI 이동
	virtual void OnInterfaceMove(FIntPoint MoveDirection) {}
};
