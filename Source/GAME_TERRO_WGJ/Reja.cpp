// Fill out your copyright notice in the Description page of Project Settings.


#include "Reja.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "UObject/ConstructorHelpers.h"

AReja::AReja()
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> HoeMesh(TEXT("StaticMesh'/Game/OldCatacombs/Meshes/SM_Door_A.SM_Door_A'"));
	if (HoeMesh.Succeeded())
	{
		MeshHerramienta->SetStaticMesh(HoeMesh.Object);
	}
	Identificador = TEXT("Reja");

	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->SetupAttachment(RootComponent);
	AudioComp->bAutoActivate = false;

	// Cargar el sonido por código (también lo puedes asignar en el editor)
	static ConstructorHelpers::FObjectFinder<USoundBase> SND(
		TEXT("SoundWave'/Game/MoreSounds/Sonidos/Interaccion/SubirBajarObjeto/subirRejas.subirRejas'"));
	if (SND.Succeeded())
	{
		SndSubir = SND.Object;
	}
}

void AReja::BeginPlay()
{
	Super::BeginPlay();
}

void AReja::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bSubiendo) return;

	const float t = (GetWorld()->GetTimeSeconds() - T0) / FMath::Max(0.01f, DuracionSubida);
	const float a = FMath::Clamp(t, 0.f, 1.f);

	const FVector L = FMath::Lerp(LocIni, LocFin, a);
	const FVector S = FMath::Lerp(ScaleIni, ScaleFin, a);
	SetActorLocation(L);
	SetActorScale3D(S);

	if (a >= 3.f)
	{
		bSubiendo = false;
		if (AudioComp && AudioComp->IsPlaying())
			AudioComp->Stop();
	}
}

void AReja::SubirReja()
{
	if (bSubiendo) return;

	LocIni = GetActorLocation();
	LocFin = LocIni + FVector(0.f, 0.f, AlturaSubida);

	ScaleIni = GetActorScale3D();
	ScaleFin = FVector(ScaleIni.X, ScaleIni.Y, ScaleIni.Z * FactorEscalaZ);

	T0 = GetWorld()->GetTimeSeconds();
	bSubiendo = true;

	ReproducirSonidoReja();
}

void AReja::ReproducirSonidoReja()
{
	if (AudioComp && SndSubir)
	{
		AudioComp->SetSound(SndSubir);
		AudioComp->Play(0.f);
	}
}