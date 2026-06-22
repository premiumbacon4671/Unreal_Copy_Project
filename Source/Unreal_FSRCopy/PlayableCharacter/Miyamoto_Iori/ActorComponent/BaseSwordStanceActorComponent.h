// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "PublicUse/AttackCombatStruct/AttackCombatStruct.h"
#include "DataAsset/PrimaryDataAsset/SkillDataAsset/SkillDataAsset.h"
#include "BaseSwordStanceActorComponent.generated.h"

//enum class EAttackVariety;
UENUM(BlueprintType)
enum class EWeaponVFXTarget : uint8
{
	RightWeapon,
	LeftWeapon,
	BothWeapon
};

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
	//응격 몽타주
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAnimMontage> CounterAttackMontage;
#pragma endregion
	bool IsUnlockSwordStance{ false };
	UPROPERTY(EditAnywhere, Category = "Stance")
	bool IsUseStance{ false };

	UPROPERTY(EditAnywhere, Category = "AttackData")
	TArray<struct FAttackData> NormalAttackData;
	UPROPERTY(EditAnywhere, Category = "AttackData")
	TArray<struct FAttackData> HeavyAttackData;
	UPROPERTY(EditAnywhere, Category = "AttackData")
	FAttackData CounterAttackData;

	UPROPERTY(EditAnywhere, Category = "AttackData")
	TObjectPtr<USkillDataAsset> HikenDataAsset;

	//공격 중 연속된 섹션과 다른 공격으로 넘어갈 때, 다음 공격으로 넘어갈 수 있는 섹션 인덱스
	UPROPERTY(EditAnywhere, Category = "Attack")
	TMap<int32, int32> ComboAttackIndexMap;

	UPROPERTY(VisibleAnywhere, Category = "Attack")
	int NormalAttackSectionIndex{ 0 };

	//선입력 가능 확인 여부와 선입력 확인 여부 기능 분리 예정
	// 
	bool IsPossibleNextAttack{ false };
	// 
	//선입력 제어
	UPROPERTY(EditAnywhere, Category = "Attack")
	bool bIsAttackQueued { false };
	//선입력 상태 제어
	UPROPERTY(EditAnywhere, Category = "Attack")
	bool bCanReceiveInput { false };
	//다음 공격으로 넘어가는 중인지 제어
	UPROPERTY(EditAnywhere, Category = "Attack")
	bool bIsNextAttackTransitioning { false };

	UPROPERTY(EditAnywhere, Category = "Attack")
	TObjectPtr<UAnimMontage> NextAttackMontage;
	UPROPERTY(EditAnywhere, Category = "Attack")
	FName NextAttackName;
	UPROPERTY(EditAnywhere, Category = "Attack")
	float AmountAttackSpeed{ 0.0f };
	const float BasicAttackSpeed{ 1.0f };

	//특수 공격(필살기, 차징, 1회용)
	//공격력 한번 사용하고 초기화
	UPROPERTY(EditAnywhere, Category = "Attack")
	int SpeicalAttackPower{ 0 };

	//차	지 공격
	float MaxChargeTime{ 4.0f };
	float CurrentChargeTime{ 0.0f };
	float ChargeStartTime{ 0.0f };
	bool IsCharging{ false };


	UPROPERTY(EditDefaultsOnly, Category = "Weapon Config")
	EWeaponVFXTarget WeaponVFXTarget{ EWeaponVFXTarget::RightWeapon };

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
	FName GetCurHeavyAttackSectionName();
	void ResetNormalAttackSectionIndex() { NormalAttackSectionIndex = 0; }
	bool IsAttacking();
	UAnimMontage* GetNormalAttackMontage() const { return NormalAttackMontage; }
	UAnimMontage* GetHeavyAttackMontage() const { return HeavyAttackMontage; }
	void PlayNextAttackMontage();
	
	void ResetNextAttack();
	void ResetAttackInfo();
	
	bool GetbIsNextAttackTransitioning() const { return bIsNextAttackTransitioning; }
	void ResetbIsNextAttackTransitioning() { bIsNextAttackTransitioning = false; }

	virtual void ReleaseSwordStance();
	virtual void InitSwordStance();
	void SetIsUseStance(bool isUse) { IsUseStance = isUse; }

	virtual int SwordStanceBeforeUpdateHp(int Damage);
	virtual void SwordStanceAfterUpdateHp(int Damage);
	virtual int SwordStanceUpdateAttack();


	bool GetIsPlayHeavyAttackMontage();

	int GetSpeicalAttackPower() const { return SpeicalAttackPower; }
	void ResetSpeicalAttackPower() { SpeicalAttackPower = 0; }
	float GetDamageMultiplier(EAttackVariety AttackVariety);

	USkillDataAsset* GetHikenDataAsset() const { return HikenDataAsset; }
	FAttackData GetAttackData(EAttackVariety AttackVariety);

	EWeaponVFXTarget GetWeaponVFXTaraget() const { return WeaponVFXTarget; }
};
