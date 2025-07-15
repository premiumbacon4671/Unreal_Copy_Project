// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseSwordStanceActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_FSRCOPY_API UBaseSwordStanceActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBaseSwordStanceActorComponent();

protected:
#pragma region Montage
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAnimMontage> NormalAttackMontage;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAnimMontage> HeavyAttackMontage;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAnimMontage> HikenMontage;
#pragma endregion

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void PlayNormalAttackMontage();
	void PlayHeavyAttackMontage();
	UAnimMontage* GetNormalAttackMontage() const { return NormalAttackMontage; }
	UAnimMontage* GetHeavyAttackMontage() const { return HeavyAttackMontage; }
};
