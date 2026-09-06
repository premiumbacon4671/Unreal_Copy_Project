// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeBase/MainMenu/MainMenuGameModeBase.h"
#include "Controller/MainMenu/MainMenuPlayerController.h"

AMainMenuGameModeBase::AMainMenuGameModeBase()
{
	DefaultPawnClass = nullptr;
	PlayerControllerClass = AMainMenuPlayerController::StaticClass();
}
