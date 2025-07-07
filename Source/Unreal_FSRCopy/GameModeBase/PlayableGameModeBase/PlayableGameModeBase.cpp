// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeBase/PlayableGameModeBase/PlayableGameModeBase.h"
#include "PlayableCharacter/Miyamoto_Iori/Miyamoto_Iori.h"
#include "Controller/MiyamotoIoriController/MiyamotoIoriController.h"

APlayableGameModeBase::APlayableGameModeBase()
{
	//수정 예정
	static ConstructorHelpers::FClassFinder<ACharacter> PlayerCharacterClass(
		TEXT("/Game/Blueprint/PlayableCharacter/MiyamotoIori/BP_MiyamotoIori.BP_MiyamotoIori_C"));
	if (PlayerCharacterClass.Succeeded())
		DefaultPawnClass = PlayerCharacterClass.Class;
	PlayerControllerClass = AMiyamotoIoriController::StaticClass();
}
