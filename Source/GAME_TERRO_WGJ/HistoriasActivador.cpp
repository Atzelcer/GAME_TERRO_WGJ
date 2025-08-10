// Fill out your copyright notice in the Description page of Project Settings.


#include "HistoriasActivador.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "HUD_terror.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Pawn.h"
#include "GAME_TERRO_WGJCharacter.h"


// Sets default values
AHistoriasActivador::AHistoriasActivador()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HistoriasActivity = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshHerramienta"));
	RootComponent = HistoriasActivity;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> LanternMesh(TEXT("StaticMesh'/Game/Cemetery_VOL1/Meshes/SM_Gravestone_01a.SM_Gravestone_01a'"));
	if (LanternMesh.Succeeded())
	{
		HistoriasActivity->SetStaticMesh(LanternMesh.Object);
	}

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Trigger->SetupAttachment(RootComponent);
	Trigger->InitBoxExtent(FVector(80.f,80.f,400.f));
	Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Trigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	Trigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Trigger->SetGenerateOverlapEvents(true);
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
    }
}

void AHistoriasActivador::setHistoria(const FText& texto)
{
	HistoriaMarkup = texto;
}

