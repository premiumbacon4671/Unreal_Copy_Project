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
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAnimMontage> CounterAttackMontage;
#pragma endregion
	bool IsUnlockSwordStance{ false };
	UPROPERTY(EditAnywhere, Category = "Stance")
	bool IsUseStance{ false };

	UPROPERTY(EditAnywhere, Category = "Montage")
	TArray<FName> NormalAttackSectionNames;
	UPROPERTY(EditAnywhere, Category = "Montage")
	TArray<FName> HeavyAttackSectionNames;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TArray<int> HeavyAttackMaxCount;
	UPROPERTY(VisibleAnywhere, Category = "Attack")
	TArray<int> HeavyAttackCount;

	//공격 중 연속된 섹션과 다른 공격으로 넘어갈 때, 다음 공격으로 넘어갈 수 있는 섹션 인덱스
	UPROPERTY(EditAnywhere, Category = "Attack")
	TMap<int32, int32> ComboAttackIndexMap;

	UPROPERTY(VisibleAnywhere, Category = "Attack")
	int NormalAttackSectionIndex{ 0 };
	UPROPERTY(EditAnywhere, Category = "Attack")
	bool IsPossibleNextAttack{ false };
	UPROPERTY(EditAnywhere, Category = "Attack")
	TObjectPtr<UAnimMontage> NextAttackMontage;
	UPROPERTY(EditAnywhere, Category = "Attack")
	FName NextAttackName;

	float AmountAttackSpeed{ 0.0f };
	const float BasicAttackSpeed{ 1.0f };

	//특수 공격(필살기, 차징, 1회용)
	//공격력 한번 사용하고 초기화
	int SpeicalAttackPower{ 0 };

	//차	지 공격
	float MaxChargeTime{ 4.0f };
	float CurrentChargeTime{ 0.0f };
	float ChargeStartTime{ 0.0f };
	bool IsCharging{ false };

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

	void SetIsUnlockSwordStance(bool isUnlock) { IsUnlockSwordStance = isUnlock; }
	bool GetIsUnlockSwordStance() const { return IsUnlockSwordStance; }

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
	virtual void ReleaseSwordStance();
	virtual void InitSwordStance();
	void SetIsUseStance(bool isUse) { IsUseStance = isUse; }

	virtual int SwordStanceBeforeUpdateHp(int Damage);
	virtual void SwordStanceAfterUpdateHp(int Damage);
	virtual int SwordStanceUpdateAttack();


	bool GetIsPlayHeavyAttackMontage();

	int GetSpeicalAttackPower() const { return SpeicalAttackPower; }
};
