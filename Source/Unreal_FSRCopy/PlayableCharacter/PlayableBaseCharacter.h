// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/AttackTraceNotify.h"
#include "PlayableBaseCharacter.generated.h"

UCLASS()
class UNREAL_FSRCOPY_API APlayableBaseCharacter : public ACharacter, public IAttackTraceNotify
{
	GENERATED_BODY()

private:
#pragma region Component
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> HeadComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> ArmComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> LegComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> FootComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> HairComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USpringArmComponent> SpringArm;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCameraComponent> Camera;
#pragma endregion

#pragma region MoveOptions
	bool isSprint{ false };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Options", meta = (AllowPrivateAccess = "true"))
	float WalkSpeed{ 600.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Options", meta = (AllowPrivateAccess = "true"))
	float SprintSpeed{ 1200.0f };
	//공격, 무기 변경 중 움직임이나 다른 행동을 못하게 하는 락
	bool bIsActionLock{ false };
public:
	bool GetIsActionLock() const { return bIsActionLock; }
	void SetIsActionLock(bool Lock);
#pragma endregion

private:
#pragma region Moveing
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAnimMontage> JumpMontage;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAnimMontage> EvadeMontage;
#pragma endregion

protected:
#pragma region protected Component
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> BodyComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> FirstWeaponComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> FirstWeaponCoverComponent;
	UPROPERTY(VisibleANywhere)
	TObjectPtr<USkeletalMeshComponent> SecondWeaponComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> SecondWeaponCoverComponent;

	UPROPERTY(VisibleAnywhere, Category = "Status")
	TObjectPtr<class UPlayableStateComponent> StatusComponent;
#pragma endregion

#pragma region CombatMode
	bool isCombatMode{ false };

	UPROPERTY(EditAnywhere, Category = "Sword Stance")
	TObjectPtr<class UBaseSwordStanceActorComponent> CurSwordStanceComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAnimMontage> EquipMontage;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAnimMontage> UnEquipMontage;

	//저스트 가드
	bool IsCanGuardConuterAttack{ false };
	FTimerHandle GuardCounterAttackTimerHale;
	//회피 및 반격
	//저스트회피 가능한 상태
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat|Dodge")
	bool bIsPerfectDodgeWindow{ false };
	// 응격(반격) 입력을 대기
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat|Counter")
	bool bIsWaitingForCounterInput{ false };
	FTimerHandle CounterInputTimerHandle;
#pragma endregion

#pragma region UI
protected:
	TSubclassOf<class UCounterAttackUI> CounterAttackWidget;
	TObjectPtr<class UCounterAttackUI> CounterAttackUI;
#pragma endregion

public:
	// Sets default values for this character's properties
	APlayableBaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void PossessedBy(AController* NewController) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void SetMoveSpeed();
	void SetSprint();
	void SetWalk();
	void PlayEvade();
	bool IsEvading() { return BodyComponent->GetAnimInstance()->Montage_IsPlaying(EvadeMontage); }
	void SetBrakingDecelerationFalling();
	bool PlayJumpMontage();
	void PlayJump();
	bool PlayMontageFullBody(TObjectPtr<UAnimMontage> Montage, FName SectionName = "", float MontageSpeed = 1.0f);
	void SetCombatMode();
	
	USkeletalMeshComponent* GetBodyComponent() const { return BodyComponent; }
	void StopMontage(TObjectPtr<UAnimMontage> Montage);

	void PostInitializeComponents() override;
	UFUNCTION()
	void AttackMontageStarted(UAnimMontage* Montage);
	UFUNCTION()
	void AttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	bool GetIsCombatMode() const { return isCombatMode; }
	bool GetIsCanGuardConuterAttack() const { return IsCanGuardConuterAttack; }
	void DisableCounterAttack() { IsCanGuardConuterAttack = false; }
	void ResetCounterAttackTimer();
	UBaseSwordStanceActorComponent* GetCurSwordStanceComponent() const { return CurSwordStanceComponent; }
	UPlayableStateComponent* GetStatusComponent() const { return StatusComponent; }

	void PCTakeDamage(int Damage);

#pragma region EquipMontageTest
	//Old Version
	virtual void PlayEquipWeaponMontage();
	virtual void WeaponEquip();
	virtual void WeaponUnEquip();

	//New Version
private:
	virtual void PlayEquipWeaponStateMontage_New(bool bIsEquip);
	
public:
	virtual void PlayEquipWeaponMontage_New() { PlayEquipWeaponStateMontage_New(true); };
	virtual void PlayUnEquipWeaponMontage_New() { PlayEquipWeaponStateMontage_New(false); };
#pragma endregion

#pragma region PerfectDodge
	void SetPerfectDodgeWindow(bool bIsPDW) { bIsPerfectDodgeWindow = bIsPDW; }
	bool GetIsPerfectDodgeWindow() const { return bIsPerfectDodgeWindow; }
	void SetWaitingForCounterInput(bool bIsWaiting) { bIsWaitingForCounterInput = bIsWaiting; }
	bool GetIsWaitingForCounterInput() const { return bIsWaitingForCounterInput; }

	//몬스터 공격이 닿았을 때
	void OnPerfectDodgeSuccess(AActor* Attacker);
	//대기 시간이 끝났을 때(반격 입력이 없는 경우)
	void EndCounterInputWindow();
#pragma endregion

#pragma region AttackTraceNotify
public:
	void AttackTrace() override;
#pragma endregion

#pragma region UI
	FKey GetCounterAttackInputKey() const;
	//입력키에 맞는 UI 아이콘을 설정하는 함수
	void InitializeIconUI();
#pragma endregion

	UFUNCTION()
	virtual void EquipMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
	virtual void UnEquipMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	public:
		// 델리게이트에 등록될 "검사관" 함수 (가상함수 아님, 자식이 건드릴 필요 없음)
		UFUNCTION()
		void OnMontageEndedGeneral(UAnimMontage* Montage, bool bInterrupted);

protected:
	// 검사가 통과되면 실행될 "실제 로직" 함수 (가상함수)
	virtual void ProcessMontageEndedGeneral(UAnimMontage* Montage, bool bInterrupted);

};
