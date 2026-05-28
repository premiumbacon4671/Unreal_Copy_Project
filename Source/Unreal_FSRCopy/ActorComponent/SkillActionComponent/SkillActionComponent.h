// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"

#include "DataAsset/PrimaryDataAsset/SkillDataAsset/SkillDataAsset.h"
#include "SkillActionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_FSRCOPY_API USkillActionComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	//이오리 마술, 서번트 공명스킬
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<USkillDataAsset>> NormalSkills;
	//이오리 비검, 서번트 보구
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkillDataAsset> HikenSkill;


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
	void SetHikenSkill(USkillDataAsset* NewSkill);
	FAttackData GetCurrentActiveSkillAttackData() const { return CurrentActiveSkill ? CurrentActiveSkill->AttackData : FAttackData(); }
	FSkillGatherSetting GetCurrentActiveSkillGatherSetting() const { return CurrentActiveSkill ? CurrentActiveSkill->GatherSetting : FSkillGatherSetting(); }

	void GatherEnemies(const FSkillGatherSetting& GatherSetting);
	void ExecuteSkill(int32 index);
	void TryExecuteSkill(USkillDataAsset* TargetSkill);
	
	void BindMontageEndedDelegate(UAnimInstance* AnimInstance);
	void OnSkillMontageEnded(UAnimMontage* Montage, bool bInterrupted);
#pragma region Production
	void PlaySkillCinematic(USkillDataAsset* TargetSkill);
	//AnimNotify에서 호출되는 함수로, 슬로우모션이 끝나는 시점에 호출되어야 함
	void EndSkillCinematic(USkillDataAsset* TargetSkill);

	UFUNCTION()
	void UpdateCameraTimeline(float Value);
	UFUNCTION()
	//카메라 타임라인이 끝나는 시점에 호출되는 함수로, 카메라를 원래 위치로 되돌리는 역할을 함
	void OnCameraTimelineFinished();
#pragma endregion
};
