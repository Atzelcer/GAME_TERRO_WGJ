// Copyright Epic Games, Inc. All Rights Reserved.

#include "GAME_TERRO_WGJGameMode.h"
#include "GAME_TERRO_WGJCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGAME_TERRO_WGJGameMode::AGAME_TERRO_WGJGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
