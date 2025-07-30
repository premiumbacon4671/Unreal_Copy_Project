// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayableBaseCharacter.generated.h"

UCLASS()
class UNREAL_FSRCOPY_API APlayableBaseCharacter : public ACharacter
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
#pragma endregion

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
#pragma endregion

#pragma region CombatMode
	bool isCombatMode{ false };

	UPROPERTY(EditAnywhere, Category = "Sword Stance")
	TObjectPtr<class UBaseSwordStanceActorComponent> CurSwordStanceComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAnimMontage> EquipMontage;
	
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
	virtual void PlayEquipWeaponMontage();
	virtual void WeaponEquip();
	virtual void WeaponUnEquip();
	USkeletalMeshComponent* GetBodyComponent() const { return BodyComponent; }
	void StopMontage(TObjectPtr<UAnimMontage> Montage);

	void PostInitializeComponents() override;
	UFUNCTION()
	void AttackMontageStarted(UAnimMontage* Montage);
	UFUNCTION()
	void AttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	bool GetIsCombatMode() const { return isCombatMode; }
	UBaseSwordStanceActorComponent* GetCurSwordStanceComponent() const { return CurSwordStanceComponent; }
};
