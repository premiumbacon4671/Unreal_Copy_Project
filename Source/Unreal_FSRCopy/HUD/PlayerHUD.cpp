// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/SwordStanceUI.h"
#include "UI/PlayableStatusUI.h"
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
}

void APlayerHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void APlayerHUD::SetSwordStanceUIVisibility(ESlateVisibility eVisibility)
{
	SwordStanceUI->SetVisibility(eVisibility);
}

void APlayerHUD::SelectSwordStance(FIntPoint MoveIndex)
{
	SwordStanceUI->SelectSwordStance(MoveIndex);
}

void APlayerHUD::StartedSwordStanceUI()
{
	SwordStanceUI->StartedSwordStanceUI();
}

void APlayerHUD::EndedSwordStanceUI(AMiyamoto_Iori* Miyamoto)
{
	SwordStanceUI->EndedSwordStanceUI(Miyamoto);
}
