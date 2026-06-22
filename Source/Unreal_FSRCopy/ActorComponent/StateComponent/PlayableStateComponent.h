// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponent/StateComponent/BaseStateComponent.h"
#include "PlayableStateComponent.generated.h"

/**
 * 
 */

class APlayableBaseCharacter;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnInitializedStat, APlayableBaseCharacter*)
DECLARE_DELEGATE_OneParam(FOnCalculateHikenGauge, float)

USTRUCT(BlueprintType)
struct FPlayableStat : public FBaseStat
{
	GENERATED_BODY()
public:
	FPlayableStat() :
		Level(1),
		Experience(0),
		MaxExperience(0),
		Mat(0),
		Tec(0),
		Hiken(40),
		MaxHiken(100),
		ExtraAttackPower(0),
		ExtraDefencePower(0) {
	}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayableStat")
	int Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayableStat")
	float Experience;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayableStat")
	float MaxExperience;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayableStat")
	int Mat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayableStat")
	int Tec;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayableStat")
	float Hiken;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayableStat")
	float MaxHiken;
	//특성(버프, 디버프, 검의 형)을 통해 얻는 가변 가능한 능력치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayableStat")
	int ExtraAttackPower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayableStat")
	int ExtraDefencePower;
};

UCLASS()
class UNREAL_FSRCOPY_API UPlayableStateComponent : public UBaseStateComponent
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "PlayableStat")
	FPlayableStat PlayableStat;

	UPROPERTY(EditAnywhere, Category = "GainMultiplier")
	float HikenGainMultiplier{ 20.0f };
public:
	FOnInitializedStat OnInitializedStat;
	FOnCalculateHikenGauge OnCalculateHikenGauge;
	// Sets default values for this component's properties
	UPlayableStateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual FBaseStat& GetStat() override { return PlayableStat; }
	virtual const FBaseStat& GetStat() const override { return PlayableStat; }
	//FPlayableStat단계의 스탯을 변경하는 모든 함수는 GetPlayableStat 함수를 통해서 접근해야 한다.
	//자식 클래스도 각자의 구조체 변수를 가지기 때문에, 각자의 변수에 접근하기 위해서는 GetPlayableStat 함수를 virtual로 선언해야 한다.
	//자식 클래스가 계속 생길 수록 override계속 해줘야 하는 문제가 있음.
	virtual FPlayableStat& GetPlayableStat() { return PlayableStat; }
	virtual const FPlayableStat& GetPlayableStat() const { return PlayableStat; }

	float GetHikentPercent() const { return static_cast<float>(GetPlayableStat().Hiken) / static_cast<float>(GetPlayableStat().MaxHiken); }
	void AddExtraAttackPower(int iExtraAttackPower) { GetPlayableStat().ExtraAttackPower += iExtraAttackPower; }
	void SubExtraAttackPower(int iExtraAttackPower) { GetPlayableStat().ExtraAttackPower -= iExtraAttackPower; if (GetPlayableStat().ExtraAttackPower <= 0) GetPlayableStat().ExtraAttackPower = 0; }
	int GetExtraAttackPower() const { return GetPlayableStat().ExtraAttackPower; }
	int GetTotalAttackPower() const { return GetPlayableStat().AttackPower + GetPlayableStat().ExtraAttackPower; }
	void AddExtraDefencePower(int iExtraDefencePower) { GetPlayableStat().ExtraDefencePower += iExtraDefencePower; }
	void SubExtraDefencePower(int iExtraDefencePower) { GetPlayableStat().ExtraDefencePower -= iExtraDefencePower; if (GetPlayableStat().ExtraDefencePower <= 0) GetPlayableStat().ExtraDefencePower = 0; }
	int GetExtraDefencePower() const { return GetPlayableStat().ExtraDefencePower; }
	int GetTotalDefencePower() const { return GetPlayableStat().DefencePower + GetPlayableStat().ExtraDefencePower; }
	virtual void InitState(const FBaseStat& InBaseStat) override;
	int GetMat() const { return GetPlayableStat().Mat; }

	void CalculateHikenGauge(float AmountDamage, float TargetMaxHP);

#pragma region Buff
public:
	void ApplyAttackBuff(float BonusMultiplier, float Duration, const TArray<class UNiagaraComponent*>& InBuffVFXs);

private:
	UFUNCTION()
	void RemoveAttackBuff(int32 BuffAmount);
	FTimerHandle BuffTimerHandle;
	UPROPERTY()
	TArray<TObjectPtr<class UNiagaraComponent>> ActiveBuffVFXs;
#pragma endregion
};
