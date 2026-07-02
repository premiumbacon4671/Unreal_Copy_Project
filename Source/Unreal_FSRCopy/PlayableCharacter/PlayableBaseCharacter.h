// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/AttackTraceNotify.h"
#include "CombatZone/CombatZone.h"
#include "PlayableBaseCharacter.generated.h"

struct FPlayableStat;
enum class EWeaponVFXTarget : uint8;
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

#pragma region Status
protected:
	//향후 서번트 스탯 필요 변수 작성예정
	TObjectPtr<class UDataTable> PlayableDataTable;
#pragma endregion

#pragma region MoveOptions
private:
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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat|Guard")
	bool IsCanGuardConuterAttack{ false };
	FTimerHandle GuardCounterAttackTimerHandle;

	//회피 및 반격
	//저스트회피 가능한 상태
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat|Dodge")
	bool bIsPerfectDodgeWindow{ false };
	// 응격(반격) 입력을 대기
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat|Counter")
	bool bIsWaitingForCounterInput{ false };
	//저스트회피 타이머
	FTimerHandle PerfectDodgeTimerHandle;
	//입력 대기타이머
	FTimerHandle CounterInputTimerHandle;

	TObjectPtr<class ACharacter> LastAttacker;

	//스킬
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat|Skill")
	TObjectPtr<class USkillActionComponent> SkillActionComponent;

	//현재 전투구역
	UPROPERTY(VisibleAnywhere, Category = "CombatZone")
	TObjectPtr<ACombatZone> CurrentCombatZone;

	UPROPERTY(VisibleAnywhere, Category = "Targeting")
	TObjectPtr<class ABaseMonster> CurrentTarget;
	// 타겟팅 시 카메라가 적을 추적하는 회전 보간 속도
	UPROPERTY(VisibleAnywhere, Category = "Targeting")
	float CameraTargetingInterpSpeed{ 5.0f };
	
	// 스킬, 피격 등 다양한 이유로 입력이 일시 차단되었는지 확인하는 통합 플래그
	bool bIsContinuousInputBlocked = false;
#pragma endregion

#pragma region UI
private:
	
	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<class UWidgetComponent> CounterAttackWidgetComponent;
	TObjectPtr<class UCounterAttackUI> CounterAttackWidget;
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
	virtual void PossessedBy(AController* NewController) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void SetMoveSpeed();
	void SetSprint();
	void SetWalk();
	void PlayEvade();
	bool IsEvading() { return BodyComponent->GetAnimInstance()->Montage_IsPlaying(EvadeMontage); }
	void SetBrakingDecelerationFalling();
	void Jump() override;
	bool PlayJumpMontage();
	void PlayJump();
	bool PlayMontageFullBody(TObjectPtr<UAnimMontage> Montage, FName SectionName = "", float MontageSpeed = 1.0f);
	void SetCombatMode();

	void ResetCameraPosition();

	USkeletalMeshComponent* GetBodyComponent() const { return BodyComponent; }
	//FORCEINLINE 강제로 인라인 함수로 만듬
	FORCEINLINE USpringArmComponent* GetSpringArm() const { return SpringArm; }
	
	void StopMontage(TObjectPtr<UAnimMontage> Montage);

	void PostInitializeComponents() override;
	
	bool GetIsCombatMode() const { return isCombatMode; }
	bool GetIsCanGuardConuterAttack() const { return IsCanGuardConuterAttack; }
	void DisableCounterAttack() { IsCanGuardConuterAttack = false; }
	void ResetCounterAttackTimer();
	UBaseSwordStanceActorComponent* GetCurSwordStanceComponent() const { return CurSwordStanceComponent; }
	USkillActionComponent* GetSkillActionComponent() const { return SkillActionComponent; }
	UPlayableStateComponent* GetStatusComponent() const { return StatusComponent; }

	bool IsPlayingAttackMontage() const;
	EWeaponVFXTarget GetCurrentWeaponVFXTarget() const;
	FORCEINLINE class USkeletalMeshComponent* GetFirstWeaponMesh() const;
	FORCEINLINE class USkeletalMeshComponent* GetSecondWeaponMesh() const;

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void SetCurrentCombatZone(ACombatZone* NewCombatZone) { CurrentCombatZone = NewCombatZone; }
	ACombatZone* GetCurrentCombatZone() const { return CurrentCombatZone; }

	// 특정 상황(스킬 종료 등)에서 연속 입력 차단을 요청하는 함수
	void RequestBlockContinuousInput() { bIsContinuousInputBlocked = true; }
	void RequestUnblockContinuousInput() { bIsContinuousInputBlocked = false; }
	// 모든 연속 입력(Triggered) 함수가 통과해야 하는 '검문소' 함수
	//이 함수가 false를 반환하면 연속 입력이 차단되고, true를 반환하면 입력이 허용됨
	//현재의 bIsContinuousInputBlocked과 반대값을 반환
	bool CanProcessContinuousInput() {return !bIsContinuousInputBlocked; }
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
	void InitializeSwordStance();
#pragma endregion

#pragma region Stat
	virtual void InitializeStatus();
	virtual void TryInitializeUI();
	void ExecuteHeal(float RecoverAmount);
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
	ACharacter* GetLastAttacker() const { return LastAttacker; }
#pragma endregion

#pragma region AttackTraceNotify
public:
	void AttackTrace(EAttackVariety AttackVariety) override;
#pragma endregion

#pragma region UI
	FKey GetCounterAttackInputKey() const;
	//입력키에 맞는 UI 아이콘을 설정하는 함수
	void InitializeIconUI();
#pragma endregion

#pragma region Montage Delegate
public:
	UFUNCTION()
	virtual void EquipMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
	virtual void UnEquipMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// 델리게이트에 등록될 "검사관" 함수 (가상함수 아님, 자식이 건드릴 필요 없음)
	UFUNCTION()
	void OnMontageEndedGeneral(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void AttackMontageStarted(UAnimMontage* Montage);
	UFUNCTION()
	void AttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
#pragma endregion

#pragma region Targeting
	void LockOnBestTarget();
	void SetTargetingMode(bool bEnable);
	void OnTargetingPressed();
	void SwitchTarget(bool bSwitchRight);
	void SnapToTargetEnemy();
	bool IsCurrentTarget() { return CurrentTarget != nullptr; }
#pragma endregion

protected:
	// 검사가 통과되면 실행될 "실제 로직" 함수 (가상함수)
	virtual void ProcessMontageEndedGeneral(UAnimMontage* Montage, bool bInterrupted);

};
