// Fill out your copyright notice in the Description page of Project Settings.

#include "CajaFuerte.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "CajaFuertecpp.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"
#include "HUD_terror.h"
#include "Tenaza.h"  


ACajaFuerte::ACajaFuerte()
{
    PrimaryActorTick.bCanEverTick = false;
    Identificador = TEXT("CajaFuerte");

    static ConstructorHelpers::FObjectFinder<UStaticMesh> SafeMesh(TEXT("/Game/packComplemento/Models/Props/SM_Safe.SM_Safe"));
    if (SafeMesh.Succeeded()) MeshCompleta = SafeMesh.Object;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> ChassisMesh(TEXT("/Game/packComplemento/Models/Props/SM_Safe_Chassis.SM_Safe_Chassis"));
    if (ChassisMesh.Succeeded()) MeshChasis = ChassisMesh.Object;

    if (MeshHerramienta && MeshCompleta)
    {
        MeshHerramienta->SetStaticMesh(MeshCompleta);
        /*MeshHerramienta->SetCollisionEnabled(ECollisionEnabled::NoCollision);*/
    }

    Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
    Trigger->SetupAttachment(RootComponent);
    Trigger->InitBoxExtent(FVector(120.f,120,320.f));
    Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Trigger->SetCollisionResponseToAllChannels(ECR_Ignore);
    Trigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    Trigger->SetGenerateOverlapEvents(true);


    ClaseObjetoAExpulsar = ATenaza::StaticClass();
}

void ACajaFuerte::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    UStaticMesh* m = (bModoChasis && MeshChasis) ? MeshChasis : MeshCompleta;
    if (MeshHerramienta && m)
    {
        MeshHerramienta->SetStaticMesh(m);
        MeshHerramienta->SetWorldScale3D(EscalaComun);
    }
}

void ACajaFuerte::SetModoChasis(bool bNew)
{
    bModoChasis = bNew;
    OnConstruction(GetActorTransform());
}

void ACajaFuerte::SetEscalaComun(float s)
{
    EscalaComun = FVector(s);
    if (MeshHerramienta) MeshHerramienta->SetWorldScale3D(EscalaComun);
}

void ACajaFuerte::DesactivarColision()
{
    if (Trigger) Trigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACajaFuerte::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    if (APawn* P = Cast<APawn>(OtherActor))
        if (APlayerController* PC = Cast<APlayerController>(P->GetController()))
            if (AHUD_terror* HUD = PC->GetHUD<AHUD_terror>())
            {
                HUD->MostrarCajaFuerte(this, CodigoEsperado);
            }
}

void ACajaFuerte::NotifyActorEndOverlap(AActor* OtherActor)
{
    Super::NotifyActorEndOverlap(OtherActor);

    if (APawn* P = Cast<APawn>(OtherActor))
        if (APlayerController* PC = Cast<APlayerController>(P->GetController()))
            if (AHUD_terror* HUD = PC->GetHUD<AHUD_terror>())
            {
                HUD->OcultarCajaFuerte();
            }
}

// en CajaFuerte.cpp
void ACajaFuerte::AbrirCajaFuerte()
{
    if (bAbierta) return;
    bAbierta = true;

    // 1) cambiar al mesh abierto
    SetModoChasis(true);

    // 2) apagar colisión del trigger
    DesactivarColision();
    if (Trigger)
    {
        Trigger->SetGenerateOverlapEvents(false);
        Trigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    // 3) cerrar el widget y devolver control al juego
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        if (AHUD_terror* HUD = PC->GetHUD<AHUD_terror>())
            HUD->OcultarCajaFuerte();

        PC->SetPause(false);
        PC->SetInputMode(FInputModeGameOnly());
        PC->bShowMouseCursor = false;
    }

    // 4) soltar el objeto
    LanzarObjeto();
}


void ACajaFuerte::FeedbackCodigoIncorrecto()
{
    // placeholder: aqui puedes reproducir sonido, parpadear UI, etc.
    UE_LOG(LogTemp, Warning, TEXT("Codigo incorrecto"));
}

void ACajaFuerte::LanzarObjeto()
{
    if (!ClaseObjetoAExpulsar) return;

    const FVector from = GetActorLocation()
        + GetActorForwardVector() * 80.f
        + FVector(0, 0, 40);
    const FRotator rot = GetActorRotation();

    FActorSpawnParameters sp;
    sp.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    AActor* item = GetWorld()->SpawnActor<AActor>(ClaseObjetoAExpulsar, from, rot, sp);
    if (!item) return;

    // intenta usar el primer StaticMeshComponent con física
    if (UPrimitiveComponent* prim = Cast<UPrimitiveComponent>(item->GetRootComponent()))
    {
        prim->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        prim->SetSimulatePhysics(true);
        prim->AddImpulse(GetActorForwardVector() * -800.f, NAME_None, true);
        prim->AddAngularImpulseInDegrees(FVector(0.f, -90.f, 0.f), NAME_None, true);
    }
}
