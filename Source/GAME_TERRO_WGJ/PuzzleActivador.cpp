// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleActivador.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "HUD_terror.h"

#include "GAME_TERRO_WGJCharacter.h"

APuzzleActivador::APuzzleActivador()
{
    PrimaryActorTick.bCanEverTick = false;

    // Mesh visible
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    RootComponent = MeshComp;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMesh(
        TEXT("StaticMesh'/Game/packComplemento/Models/Props/SM_Wooden_Box_Large_Set1.SM_Wooden_Box_Large_Set1'"));
    if (BoxMesh.Succeeded())
    {
        MeshComp->SetStaticMesh(BoxMesh.Object);
    }
    MeshComp->SetCollisionProfileName(TEXT("BlockAllDynamic"));
    MeshComp->SetGenerateOverlapEvents(false);

    // Trigger para mostrar/ocultar el widget
    Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
    Trigger->SetupAttachment(RootComponent);
    Trigger->InitBoxExtent(FVector(220.f, 220.f, 220.f));
    Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    //Trigger->SetCollisionObjectType(ECC_WorldDynamic);
    Trigger->SetCollisionResponseToAllChannels(ECR_Ignore);
    Trigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    Trigger->SetGenerateOverlapEvents(true);
}

void APuzzleActivador::BeginPlay()
{
    Super::BeginPlay();
}

void APuzzleActivador::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APuzzleActivador::SetContenido(UTexture2D* InTexture, int32 InDigit)
{
    Imagen = InTexture;
    Numero = InDigit;
}

void APuzzleActivador::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    if (AGAME_TERRO_WGJCharacter* Char = Cast<AGAME_TERRO_WGJCharacter>(OtherActor))
    {
	
        if (Char->IsLocallyControlled())
        {
            if (AController* C = Char->GetController())
            {
                if (APlayerController* PC = Cast<APlayerController>(C))
                {
                    if (AHUD_terror* HUD = PC->GetHUD<AHUD_terror>())
                    {
                        HUD->MostrarPistaPiramide(Imagen, Numero);
                        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
                            FString::Printf(TEXT("Overlap with Puzzle Activator: %s"), *Char->GetName()));
                    }
                }
            }
        }
    }
}

void APuzzleActivador::NotifyActorEndOverlap(AActor* OtherActor)
{
    Super::NotifyActorEndOverlap(OtherActor);

    if (AGAME_TERRO_WGJCharacter* Char = Cast<AGAME_TERRO_WGJCharacter>(OtherActor))
    {
        if (Char->IsLocallyControlled())
        {
            if (AController* C = Char->GetController())
            {
                if (APlayerController* PC = Cast<APlayerController>(C))
                {
                    if (AHUD_terror* HUD = PC->GetHUD<AHUD_terror>())
                    {
                        HUD->OcultarPistaPiramide();
                    }
                }
            }
        }
    }
}