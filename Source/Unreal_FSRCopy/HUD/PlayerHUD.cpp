// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/SwordStanceUI.h"
#include "UI/PlayableStatusUI.h"
#include "UI/MiyamotoSkillUI.h"
#include "UI/RecoverItemMenuUI.h"
#include "Controller/MiyamotoIoriController/MiyamotoIoriController.h"
#include "PlayableCharacter/Miyamoto_Iori/Miyamoto_Iori.h"

APlayerHUD::APlayerHUD()
{
	static ConstructorHelpers::FClassFinder<USwordStanceUI> SwordStanceWidgetClass(
		TEXT("/Game/Blueprint/PlayableCharacter/MiyamotoIori/UI/BP_SwordStanceUI.BP_SwordStanceUI_C"));
	if(SwordStanceWidgetClass.Succeeded())
		SwordStanceWidget = SwordStanceWidgetClass.Class;
	static ConstructorHelpers::FClassFinder<UPlayableStatusUI> PlayableStatusWidgetClass(
		TEXT("/Game/Blueprint/PlayableCharacter/UI/BP_PlayerStateInfoUI.BP_PlayerStateInfoUI_C"));
	if (PlayableStatusWidgetClass.Succeeded())
		PlayableStatusWidget = PlayableStatusWidgetClass.Class;
	static ConstructorHelpers::FClassFinder<UMiyamotoSkillUI> MiyamotoSkillWidgetClass(
		TEXT("/Game/Blueprint/PlayableCharacter/MiyamotoIori/UI/BP_SkillUI.BP_SkillUI_C"));
	if (MiyamotoSkillWidgetClass.Succeeded())
		MiyamotoSkillWidget = MiyamotoSkillWidgetClass.Class;
	static ConstructorHelpers::FClassFinder<URecoverItemMenuUI> RecoverItemMenuWidgetClass(
		TEXT("/Game/Blueprint/PlayableCharacter/UI/BP_RecoverItemMenuUI.BP_RecoverItemMenuUI_C"));
	if (RecoverItemMenuWidgetClass.Succeeded())
		RecoverItemMenuWidget = RecoverItemMenuWidgetClass.Class;
}

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();
	if (SwordStanceWidget)
	{
		SwordStanceUI = CreateWidget<USwordStanceUI>(GetWorld(), SwordStanceWidget);
		if (SwordStanceUI)
		{
			SwordStanceUI->AddToViewport();
			SwordStanceUI->Init(Cast<AMiyamoto_Iori>(GetOwningPlayerController()->GetPawn()));
		}
	}
	if (PlayableStatusWidget)
	{
		PlayableStatusUI = CreateWidget<UPlayableStatusUI>(GetWorld(), PlayableStatusWidget);
		if (PlayableStatusUI)
		{
			PlayableStatusUI->AddToViewport();
			PlayableStatusUI->Init(Cast<APlayableBaseCharacter>(GetOwningPlayerController()->GetPawn()));
		}
	}
	if(MiyamotoSkillWidget)
	{
		MiyamotoSkillUI = CreateWidget<UMiyamotoSkillUI>(GetWorld(), MiyamotoSkillWidget);
		if (MiyamotoSkillUI)
		{
			MiyamotoSkillUI->AddToViewport();
			//MiyamotoSkillUI->Init(Cast<AMiyamoto_Iori>(GetOwningPlayerController()->GetPawn()));
		}
	}
	if (RecoverItemMenuWidget)
	{
		RecoverItemMenuUI = CreateWidget<URecoverItemMenuUI>(GetWorld(), RecoverItemMenuWidget);
		RecoverItemMenuUI->AddToViewport();
		RecoverItemMenuUI->SetVisibility(ESlateVisibility::Hidden);
	}
}

void APlayerHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void APlayerHUD::SetSwordStanceUIVisibility(ESlateVisibility eVisibility)
{
	SwordStanceUI->SetVisibility(eVisibility);
}

void APlayerHUD::InitializeSkillUI(AMiyamoto_Iori* Miyamoto)
{
	MiyamotoSkillUI->Init(Miyamoto);
}

void APlayerHUD::SetSkillUIVisibility(ESlateVisibility eVisibility)
{
	MiyamotoSkillUI->SetVisibility(eVisibility);
}

void APlayerHUD::StartedSkillUI()
{
	MiyamotoSkillUI->StartedSkillUI();
}

void APlayerHUD::EndedSkillUI()
{
	MiyamotoSkillUI->EndedSkillUI();
}