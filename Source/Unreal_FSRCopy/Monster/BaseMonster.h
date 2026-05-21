// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Interface/AttackTraceNotify.h"
#include "PublicUse/AttackCombatStruct/AttackCombatStruct.h"
#include "CombatZone/CombatZone.h"
#include "BaseMonster.generated.h"

struct FBaseStat;
DECLARE_DELEGATE(FOnAttackMontageEndedDelegate);
UCLASS()
class UNREAL_FSRCOPY_API ABaseMonster : public ACharacter, public IAttackTraceNotify
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<class UWidgetComponent> HPBarWidgetComponent;

	FTimerHandle DeathTimerHandle;

	bool bIsInitialized{ false };

protected:
	UPROPERTY(VisibleAnywhere, Category = "Status")
	TObjectPtr<class UBaseStateComponent> StatusComponent;
	//자식 클래스에서 설정
	UPROPERTY(VisibleAnywhere, Category = "Montage")
	TObjectPtr<UAnimMontage> HitByMontage;
	UPROPERTY(VisibleAnywhere, Category = "Montage")
	TObjectPtr<UAnimMontage> DeathMontage;
	UPROPERTY(VisibleAnywhere, Category = "Montage");
	TObjectPtr<UAnimMontage> NormalAttackMontage;

	UPROPERTY(EditAnywhere, Category = "AttackData")
	FAttackData MonsterAttackData;

	TObjectPtr<ACombatZone> CurrentCombatZone;
public:
	// Sets default values for this character's properties
	ABaseMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void HitBy(int DamageAmount);
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	bool IsDead();
	void MonsterNormalAttack(class APlayableBaseCharacter* Target);

	void PostInitializeComponents() override;
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	FOnAttackMontageEndedDelegate OnAttackMontageEndedDelegate;

	void InitStat(const FBaseStat& Data);
	void SetIsInitialized(bool bValue) { bIsInitialized = bValue; }
	virtual void AttackTrace(EAttackVariety AttackVariety) override;

	float GetMonsterMaxHP() const;

	void SetCurrentCombatZone(ACombatZone* CombatZone) { CurrentCombatZone = CombatZone; }
	ACombatZone* GetCurrentCombatZone() const { return CurrentCombatZone; }
};
