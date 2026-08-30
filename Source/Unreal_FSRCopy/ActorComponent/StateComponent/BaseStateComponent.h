// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DataAsset/PrimaryDataAsset/SkillDataAsset/SkillDataAsset.h"
#include "Engine/DataTable.h"
#include "BaseStateComponent.generated.h"

USTRUCT(BlueprintType)
struct FBaseStat : public FTableRowBase
{
	GENERATED_BODY()
public:
	FBaseStat() :
		MaxHP(0.0f),
		CurHP(0.0f),
		AttackPower(0),
		DefencePower(0),
		CurrentBaseSpeed(0.0f),
		MoveSpeedMultiplier(1.0f),
		Mesh(nullptr) {

	}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float CurHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int AttackPower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int DefencePower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float CurrentBaseSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MoveSpeedMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	TObjectPtr<class USkeletalMesh> Mesh;
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateHp, float, Percent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateHpSignature, UBaseStateComponent*, SenderComponent, float, NewHPPercent);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREAL_FSRCOPY_API UBaseStateComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Stat")
	FBaseStat BaseStat;

	//받은 디버프
	TMap<EDebuffType, FTimerHandle> DebuffTimerHandles;

private:
	//플레이어가 현재 가지고 있는 타격 시 받을 디버프 목록
	UPROPERTY()
	TArray<FDebuffData> ActiveOnHitDebuffs;
	//사용하는 디버프 타이머 관리용 맵
	TMap<EDebuffType, FTimerHandle> OnHitBuffTimerHandles;

public:
	// Sets default values for this component's properties
	UBaseStateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	//캐릭터 이동속도 적용
	void RefreshMovementSpeed();
public:
	FOnUpdateHp OnUpdateHp;
	FOnUpdateHpSignature OnUpdateHpSignature;
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//자식 클래스도 각자의 구조체 변수를 가지기 때문에, 각자의 변수에 접근하기 위해서는 GetStat 함수를 virtual로 선언해야 한다.
	//FBaseStat단계의 스탯을 변경하는 모든 함수는 GetStat 함수를 통해서 접근해야 한다.
	virtual FBaseStat& GetStat() { return BaseStat; }
	virtual const FBaseStat& GetStat() const { return BaseStat; }

	float GetHPPercent() const { return static_cast<float>(GetStat().CurHP) / static_cast<float>(GetStat().MaxHP); }
	float GetMaxHP() const { return GetStat().MaxHP; }
	float GetCurHP() const { return GetStat().CurHP; }
	void RecoverHP(float RecoverAmount);

	int GetAttackPower() const { return GetStat().AttackPower; }

	void TakeDamage(int DamageAmount);
	bool IsDead() const { return GetStat().CurHP <= 0; }
	int GetDefencePower() const { return GetStat().DefencePower; }
	virtual void InitState(const FBaseStat& InBaseStat);

	void SetBaseWalkSpeed(float NewBaseSpeed);
	void ApplyDebuff(const FDebuffData& Debuff);

	void AddOnHitDebuffBuff(const FDebuffData& DebuffToApply, float BuffDuration);
	const TArray<FDebuffData>& GetActiveOnHitDebuffs() const { return ActiveOnHitDebuffs; }
};
