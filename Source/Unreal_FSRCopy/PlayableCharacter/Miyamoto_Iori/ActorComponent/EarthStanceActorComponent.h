// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayableCharacter/Miyamoto_Iori/ActorComponent/BaseSwordStanceActorComponent.h"
#include "EarthStanceActorComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREAL_FSRCOPY_API UEarthStanceActorComponent : public UBaseSwordStanceActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Earth Stance")
	int EarthStanceShield{ 0 };
	UPROPERTY(EditAnywhere, Category = "Earth Stance")
	int MaxEarthStanceShield{ 300 };

	UPROPERTY(EditAnywhere, Category = "Earth Stance")
	int ExtraDefencePower{ 0 };

	UPROPERTY(VisibleAnywhere, Category = "Earth Stance")
	float ExtraDefencePowerPercent{ 0.3f };

	FTimerHandle ShieldRechargeTimer;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	UEarthStanceActorComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void PlayCompletedHeavyAttackMontage() override;
	void PlayHeavyAttack0ChargeMontage();

	void ReleaseSwordStance() override;
	void InitSwordStance() override;

	UFUNCTION()
	void RechargeEarthStanceShield();

	int SwordStanceBeforeUpdateHp(int Damage) override;
	void SwordStanceAfterUpdateHp(int Damage) override;
	int SwordStanceUpdateAttack() override;
};
