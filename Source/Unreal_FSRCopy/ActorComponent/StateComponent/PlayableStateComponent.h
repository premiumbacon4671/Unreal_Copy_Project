// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponent/StateComponent/BaseStateComponent.h"
#include "PlayableStateComponent.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FSRCOPY_API UPlayableStateComponent : public UBaseStateComponent
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "PlayableState")
	int Level{ 1 };
	UPROPERTY(EditAnywhere, Category = "PlayableState")
	int Experience{ 0 };
	UPROPERTY(EditAnywhere, Category = "PlayableState")
	int MaxExperience{ 0 };
	UPROPERTY(EditAnywhere, Category = "PlayableState")
	int Mat;
	UPROPERTY(EditAnywhere, Category = "PlayableState")
	int Tec;
	UPROPERTY(EditAnywhere, Category = "PlayableState")
	float Hiken{ 0.0f };
	UPROPERTY(EditAnywhere, Category = "PlayableState")
	float MaxHiken{ 100.0f };

public:
	// Sets default values for this component's properties
	UPlayableStateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
