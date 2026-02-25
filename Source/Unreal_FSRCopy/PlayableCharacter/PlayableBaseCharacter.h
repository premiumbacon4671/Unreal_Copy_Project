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
#pragma region Component
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
	bool IsCanConuterAttack{ false };
	FTimerHandle CounterAttackTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Sword Stance")
	TObjectPtr<class UBaseSwordStanceActorComponent> CurSwordStanceComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAnimMontage> EquipMontage;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAnimMontage> UnEquipMontage;
	
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
	bool GetIsCanConuterAttack() const { return IsCanConuterAttack; }
	void DisableCounterAttack() { IsCanConuterAttack = false; }
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

#pragma region AttackTraceNotify
public:
	void AttackTrace() override;
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
