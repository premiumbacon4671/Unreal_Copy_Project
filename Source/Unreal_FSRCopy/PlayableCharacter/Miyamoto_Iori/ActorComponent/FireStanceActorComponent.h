// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayableCharacter/Miyamoto_Iori/ActorComponent/BaseSwordStanceActorComponent.h"
#include "FireStanceActorComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREAL_FSRCOPY_API UFireStanceActorComponent : public UBaseSwordStanceActorComponent
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	UFireStanceActorComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void PlayCompletedHeavyAttackMontage() override;
	void PlayHeavyAttack0ChargeMontage();
};
