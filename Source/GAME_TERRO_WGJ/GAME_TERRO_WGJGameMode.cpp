// Copyright Epic Games, Inc. All Rights Reserved.

#include "GAME_TERRO_WGJGameMode.h"
#include "GAME_TERRO_WGJCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "HUD_terror.h"

AGAME_TERRO_WGJGameMode::AGAME_TERRO_WGJGameMode()
{
	// set default pawn class to our C++ character with Cyberpunk Girl
	DefaultPawnClass = AGAME_TERRO_WGJCharacter::StaticClass();

	HUDClass = AHUD_terror::StaticClass();
}

void AGAME_TERRO_WGJGameMode::BeginPlay()
{
	Super::BeginPlay();
	// Ensure the HUD is created and displayed
	if (AHUD_terror* HUD = Cast<AHUD_terror>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		HUD->MostrarHUD();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("HUD initialized successfully"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("HUD not found or not of type AHUD_terror"));
		UE_LOG(LogTemp, Warning, TEXT("HUD not found or not of type AHUD_terror"));
	}
}
