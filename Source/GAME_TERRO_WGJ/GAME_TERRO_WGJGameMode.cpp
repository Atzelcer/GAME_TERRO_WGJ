// Copyright Epic Games, Inc. All Rights Reserved.

#include "GAME_TERRO_WGJGameMode.h"
#include "GAME_TERRO_WGJCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "HUD_terror.h"
#include "Engine/Engine.h"
#include "HistoriasActivador.h"
#include "CajaFuerte.h"

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
	}

	const FVector loc(9680.f, -1720.f, -6530.f);
	const FRotator rot(0.f, 90.f, 0.f);

	if (UWorld* w = GetWorld())
	{
		activador = w->SpawnActor<AHistoriasActivador>(AHistoriasActivador::StaticClass(), loc, rot);
		if (activador)
		{
			activador->setHistoria(FText::FromString(TEXT("ENCUENTRA LA MANERA DE LLEGAR A LA ALDEA")));
		}
	}


	const FVector loc1(3190.f, 5470.f, -7330.f);
	const FRotator rot1(0.f, -100.f, 0.f);

	ACajaFuerte* caja = GetWorld()->SpawnActor<ACajaFuerte>(ACajaFuerte::StaticClass(), loc1, rot1);
	if (caja)
	{
		caja->SetEscalaComun(1.75f);
		caja->SetModoChasis(false); // true = chasis, false = completa
	}

}
