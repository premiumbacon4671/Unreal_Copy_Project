// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FateGameInstanceSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_FSRCOPY_API UFateGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Category = "Servant | Saber")
	TSubclassOf<class AServantBaseCharacter> SaberCharacterClass;
	UPROPERTY(BlueprintReadOnly, Category = "Skills | Iori")
	TMap<FName, TObjectPtr<class USkillDataAsset>> IoriSkillsMap;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "Skills | Iori")
	class USkillDataAsset* FindIoriSkill(FName SkillName) const;
};
