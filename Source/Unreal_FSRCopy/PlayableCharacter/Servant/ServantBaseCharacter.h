// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayableCharacter/PlayableBaseCharacter.h"
#include "ServantBaseCharacter.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FSRCOPY_API AServantBaseCharacter : public APlayableBaseCharacter
{
	GENERATED_BODY()
public:
	// 단독 행동 중인지 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Servant|State")
	bool bIsIndependentAction = false;

public:

	AServantBaseCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void InitializeStatus() override;
};
