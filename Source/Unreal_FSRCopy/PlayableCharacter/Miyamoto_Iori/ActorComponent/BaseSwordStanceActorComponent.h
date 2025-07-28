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

	

	UPROPERTY(EditAnywhere, Category = "Attack")
	TArray<int> HeavyAttackMaxCount;
	UPROPERTY(VisibleAnywhere, Category = "Attack")
	TArray<int> HeavyAttackCount;

	UPROPERTY(VisibleAnywhere, Category = "Attack")
	int NormalAttackSectionIndex{ 0 };
	UPROPERTY(EditAnywhere, Category = "Attack")
	bool IsPossibleNextAttack{ false };
	UPROPERTY(EditAnywhere, Category = "Attack")
	TObjectPtr<UAnimMontage> NextAttackMontage;
	UPROPERTY(EditAnywhere, Category = "Attack")
	FName NextAttackName;
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
	bool GetIsPossibleNextAttack() const { return IsPossibleNextAttack; }
	void ResetIsPossibleNextAttack() { IsPossibleNextAttack = false; }
	void ResetNextAttack() { IsPossibleNextAttack = false; NextAttackMontage = nullptr; NextAttackName = ""; }
	void PlayNextAttackMontage();
	void ResetAttackInfo();
	//void ResetisUseableNormalAttack() { isUseableNormalAttack = true; }
	//void ResetNormalAttack();
	//강공격 연타시 사용
	//void ResetisUseableHeavyAttack() { isUseableHeavyAttack = true; }
	//강공격 완전 종료시 사용
	//void ResetHeavyAttack();
};
