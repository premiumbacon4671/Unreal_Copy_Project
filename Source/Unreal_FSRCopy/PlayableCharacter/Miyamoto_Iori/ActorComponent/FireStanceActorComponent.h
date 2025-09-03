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
	
private:
	float BasicAttackSpeed{ 1.0f };
	float AddAttackSpeed{ 0.0f };
	int AddAttackPower{ 0 };

protected:
	virtual void BeginPlay() override;

public:
	UFireStanceActorComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void PlayCompletedHeavyAttackMontage() override;
	void PlayHeavyAttack0ChargeMontage();

	void ReleaseSwordStance() override;
	void InitSwordStance() override;

	int SwordStanceBeforeUpdateHp(int Damage) override;
	void SwordStanceAfterUpdateHp(int Damage) override;
	void SwordStanceUpdateAttack() override;
	void SetAddStanceaAility();
};
