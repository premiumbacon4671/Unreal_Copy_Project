// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/PrimaryDataAsset/SkillDataAsset/SkillDataAsset.h"
#include "HikenSkillDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FSRCOPY_API UHikenSkillDataAsset : public USkillDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cinematic|Sequencer");
	TObjectPtr<class ULevelSequence> CinematicSequence;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cinematic|Sequencer");
	FName CharacterBindingTag = TEXT("NULL");
};
