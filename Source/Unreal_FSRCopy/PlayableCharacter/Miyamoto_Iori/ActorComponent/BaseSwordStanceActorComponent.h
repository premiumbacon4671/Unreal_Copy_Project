// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseSwordStanceActorComponent.generated.h"


UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
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

	UPROPERTY(EditAnywhere, Category = "Montage")
	TArray<FName> ExtraHeavyAttackNames;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TArray<int> HeavyAttackMaxCount;
	UPROPERTY(EditAnywhere, Category = "Montage")
	TArray<int> HeavyAttackCount;

	UPROPERTY(EditAnywhere, Category = "Montage")
	bool isUseableNormalAttack{ true };

	//강공격 가능 여부 확인
	UPROPERTY(EditAnywhere, Category = "Montage")
	bool isUseableHeavyAttack{ true };
#pragma endregion

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void PlayNormalAttackMontage();
	virtual void PlayHeavyAttackMontage();
	virtual void PlayTriggeredHeavyAttackMontage();
	virtual void PlayCompletedHeavyAttackMontage();
	UAnimMontage* GetNormalAttackMontage() const { return NormalAttackMontage; }
	UAnimMontage* GetHeavyAttackMontage() const { return HeavyAttackMontage; }
	void ResetisUseableNormalAttack() { isUseableNormalAttack = true; }
	void ResetNormalAttack();
	//강공격 연타시 사용
	void ResetisUseableHeavyAttack() { isUseableHeavyAttack = true; }
	//강공격 완전 종료시 사용
	void ResetHeavyAttack();
};
