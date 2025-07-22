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
	TArray<FName> NormalAttackSectionNames;
	UPROPERTY(EditAnywhere, Category = "Montage")
	TArray<FName> HeavyAttackSectionNames;

	UPROPERTY(VisibleAnywhere, Category = "Attack")
	int NormalAttackSectionIndex{ 0 };

	UPROPERTY(EditAnywhere, Category = "Attack")
	TArray<int> HeavyAttackMaxCount;
	UPROPERTY(VisibleAnywhere, Category = "Attack")
	TArray<int> HeavyAttackCount;
	
	UPROPERTY(VisibleAnywhere, Category = "Attack")
	bool isUseableNormalAttack{ true };

	//강공격 가능 여부 확인
	UPROPERTY(VisibleAnywhere, Category = "Attack")
	bool isUseableHeavyAttack{ true };
	//다음 공격 선입력 여부 확인
	UPROPERTY(VisibleAnywhere, Category = "Attack")
	bool isPossibleNextAttack{ false };
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

	FName GetAddCurNormalAttackSectionName();
	FName GetCurHeavyAttackSectionName() { return HeavyAttackSectionNames[NormalAttackSectionIndex]; }
	void ResetNormalAttackSectionIndex() { NormalAttackSectionIndex = 0; }
	bool IsAttacking();
	UAnimMontage* GetNormalAttackMontage() const { return NormalAttackMontage; }
	UAnimMontage* GetHeavyAttackMontage() const { return HeavyAttackMontage; }
	void ResetisUseableNormalAttack() { isUseableNormalAttack = true; }
	void ResetNormalAttack();
	//강공격 연타시 사용
	void ResetisUseableHeavyAttack() { isUseableHeavyAttack = true; }
	//강공격 완전 종료시 사용
	void ResetHeavyAttack();
};
