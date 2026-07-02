// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayableCharacter/Miyamoto_Iori/ActorComponent/BaseSwordStanceActorComponent.h"
#include "EarthStanceActorComponent.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShieldChangedSignature, float, ShieldPercent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStanceStateChanged, bool, bIsActive);
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREAL_FSRCOPY_API UEarthStanceActorComponent : public UBaseSwordStanceActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Earth Stance")
	float EarthStanceShield{ 0.0f };
	UPROPERTY(EditAnywhere, Category = "Earth Stance")
	float MaxEarthStanceShield{ 300.0f };

	UPROPERTY(EditAnywhere, Category = "Earth Stance")
	int ExtraDefencePower{ 0 };

	UPROPERTY(VisibleAnywhere, Category = "Earth Stance")
	float ExtraDefencePowerPercent{ 0.3f };

	FTimerHandle ShieldRechargeTimer;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable, Category = "Events | Stance")
	FOnShieldChangedSignature OnShieldChanged;
	UPROPERTY(BlueprintAssignable, Category = "Events | Stance")
	FOnStanceStateChanged OnStanceStateChanged;

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

	float GetShieldPercent() const { return EarthStanceShield / MaxEarthStanceShield; }
};
