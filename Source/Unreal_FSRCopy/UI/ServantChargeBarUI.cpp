// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ServantChargeBarUI.h"

#include "Components/Image.h"
#include "Components/ProgressBar.h"

void UServantChargeBarUI::Init(UTexture2D* Icon, float ChargePercent)
{
	if (Icon)
	{
		ServantIcon->SetBrushFromTexture(Icon);
	}
	UpdateServantChargeBar(ChargePercent);
}

void UServantChargeBarUI::UpdateServantChargeBar(float ChargePercent)
{
	if (ServantProgressBar)
	{
		ServantProgressBar->SetPercent(ChargePercent);
	}
	if (ChargePercent > 1.0f)
	{
		SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
	}
	else
	{
		SetColorAndOpacity(FLinearColor(0.3f, 0.3f, 0.3f, 1.0f));
	}
}
