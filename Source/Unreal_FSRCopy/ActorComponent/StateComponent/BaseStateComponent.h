// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseStateComponent.generated.h"

DECLARE_DELEGATE_OneParam(FOnTakeDamage, float);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_FSRCOPY_API UBaseStateComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "State")
	int HP{ 1000 };
	UPROPERTY(EditAnywhere, Category = "State")
	int MaxHP{ 1000 };
	UPROPERTY(EditAnywhere, Category = "State")
	int AttackPower { 100 };
	UPROPERTY(EditAnywhere, Category = "State")
	int DefencePower { 50 };

public:	
	// Sets default values for this component's properties
	UBaseStateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	FOnTakeDamage OnTakeDamage;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	float GetHPPercent() const { return static_cast<float>(HP) / static_cast<float>(MaxHP); }
	int GetAttackPower() const { return AttackPower; }
	void TakeDamage(int DamageAmount);

	int GetDefencePower() const { return DefencePower; }
};
