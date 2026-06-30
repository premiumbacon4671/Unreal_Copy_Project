// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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
	TObjectPtr<class USkeletalMesh> Mesh;
};
DECLARE_DELEGATE_OneParam(FOnUpdateHp, float)

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREAL_FSRCOPY_API UBaseStateComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Stat")
	FBaseStat BaseStat;

public:
	// Sets default values for this component's properties
	UBaseStateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	FOnUpdateHp OnUpdateHp;
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
};
