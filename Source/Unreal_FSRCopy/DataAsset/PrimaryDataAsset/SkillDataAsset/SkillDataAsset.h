// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PublicUse/AttackCombatStruct/AttackCombatStruct.h"
#include "SkillDataAsset.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ESkillCostType : uint8
{
	None,
	Gem, //미야모토 이오리 스킬
	LinkSkillBall, //서번트 공명스킬
	Hiken
};

UENUM(BlueprintType)
enum class ESkillCastType : uint8
{
	Instant, //즉시 시전
	Charge, //차지 시전
	Continuous //지속 시전
};

UENUM(BlueprintType)
enum class ESkillWeaponRequirement : uint8
{
	RequireEquipment, //장비 필요
	RequireNoEquipment, //장비 불필요
};

UENUM(BlueprintType)
enum class EDebuffType : uint8
{
	None,
	Slow,
	AttackDown,
	DefenseDown,
	Stun
};

USTRUCT(BlueprintType)
struct FDebuffData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debuff")
	EDebuffType DebuffType{ EDebuffType::None };

	//지속시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debuff")
	float Duration{ 0.0f };
	//디버프 수치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debuff")
	float Value{ 0.0f };
	//디버프 확률
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debuff")
	float ApplyChance{ 100.0f };
};

//스킬 사용시 몬스터 모으기 설정 구조체
USTRUCT(BlueprintType)
struct FSkillGatherSetting
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	bool bIsGathering{ false };
	//플레이어 앞 몬스터를 모을 지점
	UPROPERTY(EditAnywhere)
	float ForwardOffset{ 300.0f };
	//몬스터를 모을 반경
	UPROPERTY(EditAnywhere)
	float GatherRadius{ 50.0f };
};

USTRUCT(BlueprintType)
struct FSkillCameraSetting
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	//스킬 사용시 카메라 효과 적용 여부
	bool bIsCameraEffect{ false };
	UPROPERTY(EditAnywhere)
	//줌인할 거리
	float TargetArmLength{ 100.0f };
	UPROPERTY(EditAnywhere)
	//카메라가 플레이어를 바라보는 각도
	//플레이어 정면에서 바라봄
	FRotator SocketRotation{ 0.0f, -180.0f, 0.0f };
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> CameraCurve;
};

UCLASS()
class UNREAL_FSRCOPY_API USkillDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Name")
	FName SkillName;
	UPROPERTY(EditAnywhere, Category = "ID")
	FName SkillID;
	UPROPERTY(EditAnywhere, Category = "Icon")
	TObjectPtr<class UTexture2D> Icon;
	UPROPERTY(EditAnywhere, Category = "CastType")
	ESkillCastType CastType = ESkillCastType::Instant;
	UPROPERTY(EditAnywhere, Category = "CastType", meta = (EditCondition = "CastType == ESkillCastType::Charge || CastType == ESkillCastType::Continuous", EditConditionHides))
	float ChargeTime = 0.0f; //차지 시전일 때 필요한 차지 시간
	UPROPERTY(EditAnywhere, Category = "WeaponRequirement")
	ESkillWeaponRequirement WeaponRequirement = ESkillWeaponRequirement::RequireEquipment;
	UPROPERTY(EditAnywhere, Category = "Visual")
	TObjectPtr<class UAnimMontage> SkillMontage;
	UPROPERTY(EditAnywhere, Category = "GatherSetting")
	FSkillGatherSetting GatherSetting;
	UPROPERTY(EditAnywhere, Category = "Cost")
	ESkillCostType CostType = ESkillCostType::None;
	UPROPERTY(EditAnywhere, Category = "Cost")
	int32 CostAmount = 0;
	
	UPROPERTY(EditAnywhere, Category = "AttackData")
	FAttackData AttackData;
	
	UPROPERTY(EditAnywhere, Category = "Debuff")
	TArray<FDebuffData> OnHitDebuffs;

	UPROPERTY(EditAnywhere, Category = "Camera")
	FSkillCameraSetting CameraSetting;

	UPROPERTY(EditAnywhere, Category = "Cinematic")
	bool bUseTimeDilation{ false };
	//meta = (EditCondition = "bUseTimeDilation") 위의 bUseTimeDilation이 true일 때만 편집 가능
	UPROPERTY(EditAnywhere, Category = "Cinematic", meta = (EditCondition = "bUseTimeDilation"));
	float CustomTimeDilationValue{ 0.05f };

	UPROPERTY(EditAnywhere, Category = "VFX")
	TObjectPtr<class UNiagaraSystem> StartVFX;
	UPROPERTY(EditAnywhere, Category = "VFX")
	TObjectPtr<class UNiagaraSystem> LoopVFX;
	UPROPERTY(EditAnywhere, Category = "VFX")
	TObjectPtr<class UNiagaraSystem> EndVFX;
};
