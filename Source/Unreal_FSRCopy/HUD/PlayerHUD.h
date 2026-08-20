// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
enum class ESWORDSTANCE : uint8;
UCLASS()
class UNREAL_FSRCOPY_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
	
private:
	TSubclassOf<class USwordStanceUI> SwordStanceWidget;
	TObjectPtr<class USwordStanceUI> SwordStanceUI;
	TSubclassOf<class UPlayableStatusUI> PlayableStatusWidget;
	TObjectPtr<class UPlayableStatusUI> PlayableStatusUI;
	TSubclassOf<class UMiyamotoSkillUI> MiyamotoSkillWidget;
	TObjectPtr<class UMiyamotoSkillUI> MiyamotoSkillUI;
	TSubclassOf<class URecoverItemMenuUI> RecoverItemMenuWidget;
	TObjectPtr<class URecoverItemMenuUI> RecoverItemMenuUI;
	TSubclassOf<class ULinkSkillUI> LinkSkillWidget;
	TObjectPtr<class ULinkSkillUI> LinkSkillUI;
public:
	APlayerHUD();
	void BeginPlay() override;
	void PostInitializeComponents() override;
	void SetSwordStanceUIVisibility(ESlateVisibility eVisibility);
	void InitializePlayableStatusUI(class APlayableBaseCharacter* PlayableCharacter);
	void InitializeSwordStanceUI(class AMiyamoto_Iori* Miyamoto);
	void InitializeSkillUI(class AMiyamoto_Iori* Miyamoto);
	void SetSkillUIVisibility(ESlateVisibility eVisibility);
	void StartedSkillUI();
	void EndedSkillUI();

	void InitializeLinkSkillUI(class AServantBaseCharacter* Servant);
	void SetLinkSkillUIVisibility(ESlateVisibility eVisibility);

	void InitializeServantGaugeUI(class AServantBaseCharacter* Servant);


	USwordStanceUI* GetSwordStanceUI() { return SwordStanceUI; }
	UPlayableStatusUI* GetPlayableStatusUI() { return PlayableStatusUI; }
	UMiyamotoSkillUI* GetMiyamotoSkillUI() { return MiyamotoSkillUI; }
	URecoverItemMenuUI* GetRecoverItemMenuUI() { return RecoverItemMenuUI; }
	ULinkSkillUI* GetLinkSkillUI() { return LinkSkillUI; }
};
