// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Interface/AttackTraceNotify.h"
#include "PublicUse/AttackCombatStruct/AttackCombatStruct.h"
#include "CombatZone/CombatZone.h"
#include "BaseMonster.generated.h"

struct FMonsterStat;
DECLARE_DELEGATE(FOnAttackMontageEndedDelegate);
UCLASS()
class UNREAL_FSRCOPY_API ABaseMonster : public ACharacter, public IAttackTraceNotify
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<class UWidgetComponent> HPBarWidgetComponent;
	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<class UWidgetComponent> LockOnMarkerWidgetComponent;

	FTimerHandle DeathTimerHandle;

	bool bIsInitialized{ false };

protected:
	UPROPERTY(VisibleAnywhere, Category = "Status")
	TObjectPtr<class UMonsterStateComponent> StatusComponent;
	//자식 클래스에서 설정
	UPROPERTY(VisibleAnywhere, Category = "Montage")
	TObjectPtr<UAnimMontage> HitByMontage;
	UPROPERTY(VisibleAnywhere, Category = "Montage")
	TObjectPtr<UAnimMontage> DeathMontage;
	UPROPERTY(VisibleAnywhere, Category = "Montage");
	TObjectPtr<UAnimMontage> NormalAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AttackData")
	FAttackData MonsterAttackData;

	TObjectPtr<ACombatZone> CurrentCombatZone;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly);
	TObjectPtr<class APlayableBaseCharacter> CurrentTarget;
public:
	// Sets default values for this character's properties
	ABaseMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	bool IsDead();
	void MonsterNormalAttack(class APlayableBaseCharacter* Target);

	void PostInitializeComponents() override;
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	FOnAttackMontageEndedDelegate OnAttackMontageEndedDelegate;

	void InitStat(const FMonsterStat& Data);
	void SetIsInitialized(bool bValue) { bIsInitialized = bValue; }
	virtual void AttackTrace(EAttackVariety AttackVariety) override;

	float GetMonsterMaxHP() const;
	UFUNCTION()
	void UpdateHPBar(class UBaseStateComponent* SenderComponent, float Percent);

	void SetCurrentCombatZone(ACombatZone* CombatZone) { CurrentCombatZone = CombatZone; }
	ACombatZone* GetCurrentCombatZone() const { return CurrentCombatZone; }

	UMonsterStateComponent* GetMonsterStateComponent() { return StatusComponent; }

	void SetLockOnMarkerVisibility(bool bShow);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	const FAttackData& GetMonsterAttackDataBP() const { return MonsterAttackData; }
	FAttackData GetMonsterAttackData() const { return MonsterAttackData; }

#pragma region AI
	void SetCurrentTarget(class APlayableBaseCharacter* Target) { CurrentTarget = Target; }
	APlayableBaseCharacter* GetCurrentTarget() const { return CurrentTarget; }
	void ClearCurrentTarget() { CurrentTarget = nullptr; }
	void SetStrafeMovementMode(bool bIsStrafing);
	void ClearLockOnTargetAI();
#pragma endregion
};
