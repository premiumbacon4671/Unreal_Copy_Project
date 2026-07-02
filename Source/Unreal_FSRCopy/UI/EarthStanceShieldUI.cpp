// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EarthStanceShieldUI.h"
#include "Components/ProgressBar.h"

#include "PlayableCharacter/Miyamoto_Iori/ActorComponent/EarthStanceActorComponent.h"


void UEarthStanceShieldUI::NativeConstruct()
{
	Super::NativeConstruct();
}

void UEarthStanceShieldUI::HandleShieldChanged(float ShieldPercent)
{
	if(ShieldProgressBar)
		ShieldProgressBar->SetPercent(ShieldPercent);
}

void UEarthStanceShieldUI::HandleStanceStateChanged(bool bIsActive)
{
	if (bIsActive)
	{
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UEarthStanceShieldUI::InitShieldUI(UEarthStanceActorComponent* InEarthStanceActorComponent)
{
	if(!InEarthStanceActorComponent)
		return;
	if(EarthStanceActorComponent)
	{
		EarthStanceActorComponent->OnShieldChanged.RemoveDynamic(this, &UEarthStanceShieldUI::HandleShieldChanged);
	}
	EarthStanceActorComponent = InEarthStanceActorComponent;
	EarthStanceActorComponent->OnShieldChanged.AddDynamic(this, &UEarthStanceShieldUI::HandleShieldChanged);
	EarthStanceActorComponent->OnStanceStateChanged.AddDynamic(this, &UEarthStanceShieldUI::HandleStanceStateChanged);
	float InitialShieldPercent = EarthStanceActorComponent->GetShieldPercent();
	HandleShieldChanged(InitialShieldPercent);
}
