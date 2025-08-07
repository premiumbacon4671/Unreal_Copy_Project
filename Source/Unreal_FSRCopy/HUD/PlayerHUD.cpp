// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/SwordStanceUI.h"
#include "Controller/MiyamotoIoriController/MiyamotoIoriController.h"
#include "PlayableCharacter/Miyamoto_Iori/Miyamoto_Iori.h"

APlayerHUD::APlayerHUD()
{
	static ConstructorHelpers::FClassFinder<USwordStanceUI> SwordStanceWidgetClass(
		TEXT("/Game/Blueprint/PlayableCharacter/MiyamotoIori/UI/BP_SwordStanceUI.BP_SwordStanceUI_C"));
	if(SwordStanceWidgetClass.Succeeded())
		SwordStanceWidget = SwordStanceWidgetClass.Class;
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
