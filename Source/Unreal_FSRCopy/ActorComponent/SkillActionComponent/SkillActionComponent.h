// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"

#include "DataAsset/PrimaryDataAsset/SkillDataAsset/SkillDataAsset.h"
#include "SkillActionComponent.generated.h"

// 몬스터 오프셋 고정용 구조체
struct FGatherTargetInfo
{
	TWeakObjectPtr<class ABaseMonster> TargetMonster;
	FVector RelativeOffset;
};
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_FSRCOPY_API USkillActionComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	//이오리 마술, 서번트 공명스킬
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<USkillDataAsset>> NormalSkills;
	//이오리 비검, 서번트 보구
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkillDataAsset> HikenSkill;

	float CurrentChargeTime{ 0.0 };
	float ChargeStartTime{ 0.0 };
	bool IsCharging{ false };

	FTimerHandle GatherTimerHandle;
	TArray<FGatherTargetInfo> ActiveGatherTargets;
#pragma region Production
private:
	float OriginalTimeDilation{ 1.0f };
	float DefaultCameraArmLength{ 350.0f };
	FRotator DefaultCameraRotation{ FRotator::ZeroRotator};
	UPROPERTY()
	TObjectPtr<USkillDataAsset> CurrentActiveSkill;
	UPROPERTY()
	TObjectPtr<USkillDataAsset> CurrentCinematicSkill;
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UTimelineComponent> CameraTimeline;

	FOnTimelineFloat CameraProgressDelegate;
	bool bIsTimelineTrackInitialized{ false };

	UPROPERTY()
	TObjectPtr<class ULevelSequencePlayer> ActiveSequencePlayer;
	TObjectPtr<class APlayableBaseCharacter> OwnerCharacter;

#pragma endregion
public:	
	// Sets default values for this component's properties
	USkillActionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetNormalSkill(int32 index, USkillDataAsset* NewSkill);
	USkillDataAsset* GetNormalSkill(int32 index) const {return NormalSkills.IsValidIndex(index) ? NormalSkills[index] : nullptr; }
	void SetHikenSkill(USkillDataAsset* NewSkill);
	USkillDataAsset* GetCurrentSKillData() { return CurrentActiveSkill; }
	FAttackData GetCurrentActiveSkillAttackData() const { return CurrentActiveSkill ? CurrentActiveSkill->AttackData : FAttackData(); }
	FSkillGatherSetting GetCurrentActiveSkillGatherSetting() const { return CurrentActiveSkill ? CurrentActiveSkill->GatherSetting : FSkillGatherSetting(); }

	//Skill 시퀀서에세 사용하기 위해 선언
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void GatherEnemies();
	void UpdateGathering();
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void EndGathering();
	bool ExecuteSkill(int32 index);
	bool TryExecuteSkill(USkillDataAsset* TargetSkill);
	bool HasEnoughResource(USkillDataAsset* TargetSkill) const;
	void ConsumeResource(USkillDataAsset* TargetSkill);

	UFUNCTION()
	void FireProjectileSkill(USkillDataAsset* TargetSkill);
	UFUNCTION()
	void FireBuffSkill(USkillDataAsset* TargetSkill);
	UFUNCTION()
	void FireRecoverSkill(USkillDataAsset* TargetSkill);

	void PlayTriggerSkillMontage();
	void PlayCompletedSkillMontage();
	void PlayEndSkillMontage(float ChargeRatio);
	
	void BindSkillMontageDelegate(UAnimInstance* AnimInstance);
	UFUNCTION()
	void OnSkillMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	bool IsPlayingCinematic() const;
#pragma region Production
	void PlaySkillCinematic(USkillDataAsset* TargetSkill);
	//AnimNotify에서 호출되는 함수로, 슬로우모션이 끝나는 시점에 호출되어야 함
	//시퀀서에서 사용하기 위해 UFUNCTION 선언
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void EndSkillCinematic(USkillDataAsset* TargetSkill = nullptr);

	//BeginPlay()에서 타임라인 컴포넌트를 생성하고, 타임라인이 업데이트될 때 호출되는 함수를 바인딩하는 로직이 필요합니다.
	UFUNCTION()
	void UpdateCameraTimeline(float Value);
	UFUNCTION()
	//카메라 타임라인이 끝나는 시점에 호출되는 함수로, 카메라를 원래 위치로 되돌리는 역할을 함
	void OnCameraTimelineFinished();
	UFUNCTION()
	void OnSkillCinematicFinished();
#pragma endregion
};
