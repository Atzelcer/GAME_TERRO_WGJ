// Fill out your copyright notice in the Description page of Project Settings.


#include "Tenaza.h"
#include "Components/SphereComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Reja.h"
#include "GAME_TERRO_WGJCharacter.h"
#include "Sound/SoundBase.h"

ATenaza::ATenaza()
{
	PrimaryActorTick.bCanEverTick = true;
	// Set the mesh for the tongs
	static ConstructorHelpers::FObjectFinder<UStaticMesh> TongsMesh(TEXT("StaticMesh'/Game/herramientasMega/Meshes/13_Pliers/SM_Pliers.SM_Pliers'"));
	if (TongsMesh.Succeeded())
	{
		MeshHerramienta->SetStaticMesh(TongsMesh.Object);
	}
	Identificador = TEXT("Tenaza");

	if (MeshHerramienta)
	{
		MeshHerramienta->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// Trigger de recogida
	Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
	Trigger->SetupAttachment(RootComponent);
	Trigger->InitSphereRadius(70.f);
	Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Trigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	Trigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Trigger->SetGenerateOverlapEvents(true);

	// Sonido de recoger
	static ConstructorHelpers::FObjectFinder<USoundBase> SND(
		TEXT("SoundWave'/Game/MoreSounds/Sonidos/Interaccion/Agarrar/agarrarObjeto.agarrarObjeto'"));
	if (SND.Succeeded())
	{
		SndAgarrar = SND.Object;
	}
}
void ATenaza::BeginPlay()
{
	Super::BeginPlay();

	if (!RejaObjetivo)
	{
		const FVector  Loc = FVector::ZeroVector;   
		const FRotator Rot = FRotator::ZeroRotator; 

		RejaObjetivo = GetWorld()->SpawnActor<AReja>(
			AReja::StaticClass(), Loc, Rot);


		if (RejaObjetivo)
		{
			RejaObjetivo->SetActorScale3D(FVector(1.f));
		}
	}

}
void ATenaza::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	// ¿El que entra es el personaje?
	if (AGAME_TERRO_WGJCharacter* Player = Cast<AGAME_TERRO_WGJCharacter>(OtherActor))
	{
		// Sonido de “agarrar”
		if (SndAgarrar)
		{
			UGameplayStatics::PlaySoundAtLocation(this, SndAgarrar, GetActorLocation());
		}

		// Dispara la reja (sonido + subida)
		if (RejaObjetivo)
		{
			RejaObjetivo->ReproducirSonidoReja();
			RejaObjetivo->SubirReja();
		}

		// Desaparece la tenaza
		Destroy();
	}
}