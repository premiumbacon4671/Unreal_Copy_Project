// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayableCharacter/PlayableBaseCharacter.h"
#include "Miyamoto_Iori.generated.h"

/**
 * 
 */

enum SwordStance
{
	EARTH,
	WATER,
};

UCLASS()
class UNREAL_FSRCOPY_API AMiyamoto_Iori : public APlayableBaseCharacter
{
	GENERATED_BODY()
	
private:
	SwordStance CurSwordStance;

public:
	AMiyamoto_Iori();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
