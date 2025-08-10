// Fill out your copyright notice in the Description page of Project Settings.


#include "HistoriasActivador.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "HUD_terror.h"
#include "GAME_TERRO_WGJCharacter.h"
#include "GameFramework/PlayerController.h"


// Sets default values
AHistoriasActivador::AHistoriasActivador()
{
    PrimaryActorTick.bCanEverTick = true;

    HistoriasActivity = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshHerramienta"));
    RootComponent = HistoriasActivity;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> LanternMesh(
        TEXT("StaticMesh'/Game/Cemetery_VOL1/Meshes/SM_Gravestone_01a.SM_Gravestone_01a'"));
    if (LanternMesh.Succeeded())
    {
        HistoriasActivity->SetStaticMesh(LanternMesh.Object);
    }

    static ConstructorHelpers::FObjectFinder<USoundBase> Papeleo(
        TEXT("SoundWave'/Game/MoreSounds/Sonidos/Interaccion/Papel/papel1.papel1'"));
    if (Papeleo.Succeeded())
    {
        SndAlEntrar = Papeleo.Object;
        if (AudioComp) AudioComp->SetSound(SndAlEntrar);
    }

    Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
    Trigger->SetupAttachment(RootComponent);
    Trigger->InitBoxExtent(FVector(80.f, 80.f, 400.f));
    Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Trigger->SetCollisionResponseToAllChannels(ECR_Ignore);
    Trigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    Trigger->SetGenerateOverlapEvents(true);

    // --- Audio ---
    AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
    AudioComp->SetupAttachment(RootComponent);
    AudioComp->bAutoActivate = false;    // lo reproducimos nosotros
    AudioComp->bIsUISound = false;     // 3D por defecto
    AudioComp->SetVolumeMultiplier(AudioVolume);
}



void AHistoriasActivador::BeginPlay()
{
    Super::BeginPlay();
}

void AHistoriasActivador::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AHistoriasActivador::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    if (AGAME_TERRO_WGJCharacter* player = Cast<AGAME_TERRO_WGJCharacter>(OtherActor))
    {
        if (APlayerController* pc = Cast<APlayerController>(player->GetController()))
        {
            if (AHUD_terror* hud = pc->GetHUD<AHUD_terror>())
            {
                hud->MostrarHistoria(HistoriaMarkup);
            }
        }

        // Reproducir audio al entrar
        if (AudioComp)
        {
            if (SndAlEntrar) AudioComp->SetSound(SndAlEntrar);
            AudioComp->SetVolumeMultiplier(AudioVolume);
            if (AudioComp->Sound && !AudioComp->IsPlaying())
            {
                AudioComp->Play(0.f);
            }
        }
    }
}

void AHistoriasActivador::NotifyActorEndOverlap(AActor* OtherActor)
{
    Super::NotifyActorEndOverlap(OtherActor);

    if (AGAME_TERRO_WGJCharacter* player = Cast<AGAME_TERRO_WGJCharacter>(OtherActor))
    {
        if (APlayerController* pc = Cast<APlayerController>(player->GetController()))
        {
            if (AHUD_terror* hud = pc->GetHUD<AHUD_terror>())
            {
                hud->OcultarHistoria();
            }
        }

        // Parar audio al salir
        if (AudioComp && AudioComp->IsPlaying())
        {
            AudioComp->Stop();
        }
    }
}

void AHistoriasActivador::setHistoria(const FText& texto)
{
    HistoriaMarkup = texto;
}