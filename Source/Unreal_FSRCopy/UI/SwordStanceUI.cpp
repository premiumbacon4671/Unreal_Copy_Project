// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SwordStanceUI.h"
#include "Components/Image.h"
#include "PlayableCharacter/Miyamoto_Iori/Miyamoto_Iori.h"
#include "UI/SwordStanceButtonUI.h"

void USwordStanceUI::Init(AMiyamoto_Iori& Miyamoto)
{
	if (Miyamoto.GetIsUnlockSwordStance(ESWORDSTANCE::EST_EARTH) == true)
	{
		EarthStance->SetSwordStanceVisibility(ESlateVisibility::Visible);
		if(Miyamoto.GetCurSwordStance() == ESWORDSTANCE::EST_EARTH)
		{
			EarthStance->SetHighlightVisibility(ESlateVisibility::Visible);
		}
	}
	if (Miyamoto.GetIsUnlockSwordStance(ESWORDSTANCE::EST_WATER) == true)
	{
		WaterStance->SetSwordStanceVisibility(ESlateVisibility::Visible);
		if (Miyamoto.GetCurSwordStance() == ESWORDSTANCE::EST_WATER)
		{
			WaterStance->SetHighlightVisibility(ESlateVisibility::Visible);
		}
	}
	if (Miyamoto.GetIsUnlockSwordStance(ESWORDSTANCE::EST_FIRE) == true)
	{
		FireStance->SetSwordStanceVisibility(ESlateVisibility::Visible);
		if (Miyamoto.GetCurSwordStance() == ESWORDSTANCE::EST_FIRE)
		{
			FireStance->SetHighlightVisibility(ESlateVisibility::Visible);
		}
	}
	if (Miyamoto.GetIsUnlockSwordStance(ESWORDSTANCE::EST_WIND) == true)
	{
		WindStance->SetSwordStanceVisibility(ESlateVisibility::Visible);
		if (Miyamoto.GetCurSwordStance() == ESWORDSTANCE::EST_WIND)
		{
			WindStance->SetHighlightVisibility(ESlateVisibility::Visible);
		}
	}
	if (Miyamoto.GetIsUnlockSwordStance(ESWORDSTANCE::EST_VOID) == true)
	{
		VoidStance->SetSwordStanceVisibility(ESlateVisibility::Visible);
		if (Miyamoto.GetCurSwordStance() == ESWORDSTANCE::EST_VOID)
		{
			VoidStance->SetHighlightVisibility(ESlateVisibility::Visible);
		}
	}
}

void USwordStanceUI::NativeConstruct()
{
	Super::NativeConstruct();
	Buttons.SetNum(3);
	Buttons[0].SetNum(3);
	Buttons[1].SetNum(3);
	Buttons[2].SetNum(3);
	if (SwordButtonClass)
	{
		EarthStance = CreateWidget<USwordStanceButtonUI>(GetWorld(), SwordButtonClass);
		//EarthStance->AddToViewport();
		Buttons[2][1] = EarthStance;
		WaterStance = CreateWidget<USwordStanceButtonUI>(GetWorld(), SwordButtonClass);
		//WaterStance->AddToViewport();
		Buttons[1][2] = WaterStance;
		VoidStance = CreateWidget<USwordStanceButtonUI>(GetWorld(), SwordButtonClass);
		//VoidStance->AddToViewport();
		Buttons[1][1] = VoidStance;
		FireStance = CreateWidget<USwordStanceButtonUI>(GetWorld(), SwordButtonClass);
		//FireStance->AddToViewport();
		Buttons[1][0] = FireStance;
		WindStance = CreateWidget<USwordStanceButtonUI>(GetWorld(), SwordButtonClass);
		//WindStance->AddToViewport();
		Buttons[0][1] = WindStance;
	}
}

bool USwordStanceUI::Initialize()
{
	Super::Initialize();
	EarthStance->SetColor(TEXT("#AD8C00FF"));
	WaterStance->SetColor(TEXT("#00B9E3FF"));
	VoidStance->SetColor(TEXT("#84349CFF"));
	FireStance->SetColor(TEXT("#CA0000FF"));
	WindStance->SetColor(TEXT("#008A00FF"));
	Init(*Cast<AMiyamoto_Iori>(GetOwningPlayerPawn()));
	return false;
}
