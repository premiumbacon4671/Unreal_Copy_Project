// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SwordStanceButtonUI.h"
#include "Components/Image.h"

void USwordStanceButtonUI::SetColor(FLinearColor color)
{
	SwordStance->Brush.TintColor = color;
}
