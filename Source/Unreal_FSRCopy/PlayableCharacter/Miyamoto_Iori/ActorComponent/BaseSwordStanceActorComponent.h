// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseSwordStanceActorComponent.generated.h"

UENUM(BlueprintType)
enum class EATTACKTYPE : uint8
{
	AT_NORMALATTACK UMETA(DisplayName = "NormalAttack"),
	AT_HEAVYATTACK UMETA(DisplayName = "HeavyAttack"),
	AT_MAX
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
	
	//일반 공격 실행 여부 확인
	UPROPERTY(VisibleAnywhere, Category = "Attack")
	bool IsPlayingNormalAttack{ false };

	//강공격 실행 여부 확인
	UPROPERTY(VisibleAnywhere, Category = "Attack")
	bool IsPlayingHeavyAttack{ false };

	//선입력 가능 여부 확인 notify state에서 사용
	UPROPERTY(EditAnywhere, Category = "Attack")
	bool IsInputPressed{ false };
	//다음 공격 선입력 여부 확인
	UPROPERTY(EditAnywhere, Category = "Attack")
	bool IsPossibleNextAttack{ false };
	UPROPERTY(EditAnywhere, Category = "Attack")
	EATTACKTYPE eNextAttackType;
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
	void SetIsPossibleNextAttack(EATTACKTYPE NextAttackType) { IsPossibleNextAttack = true; eNextAttackType = NextAttackType; }
	void PlayNextAttackMontage();
	void ResetAttackInfo();
	void CanIsInputPressed() { IsInputPressed = true; }
	void ResetIsInputPressed() { IsInputPressed = false; }
	void SetIsPlayingNormalAttack() { IsPlayingNormalAttack = true; }
	void ResetIsPlayingNormalAttack() { IsPlayingNormalAttack = false; }
	void SetIsPlayingHeavyAttack() { IsPlayingHeavyAttack = true; }
	void ResetIsPlayingHeavyAttack() { IsPlayingHeavyAttack = false; }
	//void ResetisUseableNormalAttack() { isUseableNormalAttack = true; }
	//void ResetNormalAttack();
	//강공격 연타시 사용
	//void ResetisUseableHeavyAttack() { isUseableHeavyAttack = true; }
	//강공격 완전 종료시 사용
	//void ResetHeavyAttack();
};
