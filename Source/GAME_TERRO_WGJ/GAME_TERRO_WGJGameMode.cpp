// Copyright Epic Games, Inc. All Rights Reserved.

#include "GAME_TERRO_WGJGameMode.h"
#include "GAME_TERRO_WGJCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGAME_TERRO_WGJGameMode::AGAME_TERRO_WGJGameMode()
{
	// set default pawn class to our C++ character with Cyberpunk Girl
	DefaultPawnClass = AGAME_TERRO_WGJCharacter::StaticClass();
}
