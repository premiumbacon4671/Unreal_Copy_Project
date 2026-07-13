// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayableCharacter/Miyamoto_Iori/ActorComponent/BaseSwordStanceActorComponent.h"
#include "SaberSwordStanceActorComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREAL_FSRCOPY_API USaberSwordStanceActorComponent : public UBaseSwordStanceActorComponent
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	USaberSwordStanceActorComponent();
};
