// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseStateComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_FSRCOPY_API UBaseStateComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "State")
	float HP{ 1000.0f };
	UPROPERTY(EditAnywhere, Category = "State")
	float MaxHP{ 1000.0f };
	UPROPERTY(EditAnywhere, Category = "State")
	float AttackPower { 100.0f };
	UPROPERTY(EditAnywhere, Category = "State")
	float DefencePower { 50.0f };

public:	
	// Sets default values for this component's properties
	UBaseStateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
