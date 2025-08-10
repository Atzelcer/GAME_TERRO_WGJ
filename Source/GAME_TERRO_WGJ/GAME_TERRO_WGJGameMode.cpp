// Copyright Epic Games, Inc. All Rights Reserved.

#include "GAME_TERRO_WGJGameMode.h"
#include "GAME_TERRO_WGJCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "HUD_terror.h"
#include "Engine/Engine.h"
#include "HistoriasActivador.h"
#include "CajaFuerte.h"
#include "PuzzleActivador.h"   
#include "Engine/Texture2D.h"


AGAME_TERRO_WGJGameMode::AGAME_TERRO_WGJGameMode()
{
	// set default pawn class to our C++ character with Cyberpunk Girl
	DefaultPawnClass = AGAME_TERRO_WGJCharacter::StaticClass();

	HUDClass = AHUD_terror::StaticClass();
}

void AGAME_TERRO_WGJGameMode::BeginPlay()
{
	Super::BeginPlay();

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
		caja->SetModoChasis(false);
	}

	SpawnPistasPiramide();
}

static UTexture2D* LoadTex(const TCHAR* Path)
{
	return Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, Path));
}

void AGAME_TERRO_WGJGameMode::SpawnPistasPiramide()
{
	const TArray<FVector> Locs = {
		FVector(8590.f,  8590.f, -7570.f),
		FVector(700.f,  3340.f, -7210.f),
		FVector(-1440.f, -4430.f, -6760.f),
		FVector(3940.f,  2440.f, -7220.f)
	};
	const FRotator Rot(0.f, 0.f, 0.f);

	TArray<UTexture2D*> Tex;
	Tex.Reserve(4);
	Tex.Add(LoadTex(TEXT("Texture2D'/Game/imagenes/Pir1.Pir1'")));
	Tex.Add(LoadTex(TEXT("Texture2D'/Game/imagenes/Pir2.Pir2'")));
	Tex.Add(LoadTex(TEXT("Texture2D'/Game/imagenes/Pir3.Pir3'")));
	Tex.Add(LoadTex(TEXT("Texture2D'/Game/imagenes/Pir4.Pir4'")));

	//FActorSpawnParameters SP;
	//SP.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (int32 i = 0; i < Locs.Num(); ++i)
	{
		APuzzleActivador* A = GetWorld()->SpawnActor<APuzzleActivador>(
			APuzzleActivador::StaticClass(), Locs[i], Rot);

		if (!A) continue;

		const int32 Digit = FMath::RandRange(0, 9);

		UTexture2D* Img = (Tex.IsValidIndex(i) ? Tex[i] : nullptr);

		A->SetContenido(Img, Digit);
	}
}