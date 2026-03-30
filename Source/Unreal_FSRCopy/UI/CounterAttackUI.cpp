// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CounterAttackUI.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"

void UCounterAttackUI::UpdateKeyIcon(FKey Key)
{
	if(nullptr == KeyInputImage)
		return;

	TObjectPtr<UTexture2D> IconTexture = KeyInputIconeMap.FindRef(Key);
	if(IconTexture)
	{
		KeyInputImage->SetBrushFromTexture(IconTexture);
	}
}
