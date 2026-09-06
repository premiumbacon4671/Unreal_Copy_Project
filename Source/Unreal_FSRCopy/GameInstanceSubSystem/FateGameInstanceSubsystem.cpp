// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstanceSubSystem/FateGameInstanceSubsystem.h"
#include "DataAsset/PrimaryDataAsset/SkillDataAsset/SkillDataAsset.h"
#include "PlayableCharacter/Servant/ServantBaseCharacter.h"

#include "UObject/ConstructorHelpers.h"


void UFateGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	SaberCharacterClass = LoadClass<AServantBaseCharacter>(nullptr, TEXT(
		"/Game/Blueprint/PlayableCharacter/Saber/BP_Saber.BP_Saber_C"));
	
	USkillDataAsset* IoriSkill1 = LoadObject<USkillDataAsset>(nullptr, TEXT(
		"/Game/Blueprint/PlayableCharacter/MiyamotoIori/DataAsset/DA_Flamethrower.DA_Flamethrower"));
	if (IoriSkill1)
	{
		IoriSkillsMap.Add(IoriSkill1->SkillID, IoriSkill1);
	}
	USkillDataAsset* IoriSkill2 = LoadObject<USkillDataAsset>(nullptr, TEXT(
		"/Game/Blueprint/PlayableCharacter/MiyamotoIori/DataAsset/DA_FireBall.DA_FireBall"));
	if (IoriSkill2)
	{
		IoriSkillsMap.Add(IoriSkill2->SkillID, IoriSkill2);
	}
	USkillDataAsset* IoriSkill3 = LoadObject<USkillDataAsset>(nullptr, TEXT(
		"/Game/Blueprint/PlayableCharacter/MiyamotoIori/DataAsset/DA_AttackPowerBuff.DA_AttackPowerBuff"));
	if (IoriSkill3)
	{
		IoriSkillsMap.Add(IoriSkill3->SkillID, IoriSkill3);
	}
	USkillDataAsset* IoriSkill4 = LoadObject<USkillDataAsset>(nullptr, TEXT(
		"/Game/Blueprint/PlayableCharacter/MiyamotoIori/DataAsset/DA_RecoverHp.DA_RecoverHp"));
	if (IoriSkill4)
	{
		IoriSkillsMap.Add(IoriSkill4->SkillID, IoriSkill4);
	}
}

void UFateGameInstanceSubsystem::Deinitialize()
{
	IoriSkillsMap.Empty();

	Super::Deinitialize();
}

USkillDataAsset* UFateGameInstanceSubsystem::FindIoriSkill(FName SkillName) const
{
	if (TObjectPtr<USkillDataAsset> const* FoundSkill = IoriSkillsMap.Find(SkillName))
	{
		return *FoundSkill;
	}
	return nullptr;
}
