// Fill out your copyright notice in the Description page of Project Settings.

#include "CajaFuerte.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/Pawn.h"

ACajaFuerte::ACajaFuerte()
{
    PrimaryActorTick.bCanEverTick = false;

    Identificador = TEXT("CajaFuerte");

    static ConstructorHelpers::FObjectFinder<UStaticMesh> SafeMesh(
        TEXT("/Game/packComplemento/Models/Props/SM_Safe.SM_Safe"));
    if (SafeMesh.Succeeded()) MeshCompleta = SafeMesh.Object;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> SafeChassisMesh(
        TEXT("/Game/packComplemento/Models/Props/SM_Safe_Chassis.SM_Safe_Chassis"));
    if (SafeChassisMesh.Succeeded()) MeshChasis = SafeChassisMesh.Object;

    if (MeshHerramienta && MeshCompleta)
    {
        MeshHerramienta->SetStaticMesh(MeshCompleta);
        MeshHerramienta->SetCollisionEnabled(ECollisionEnabled::NoCollision); // el trigger maneja el overlap
    }

    Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
    Trigger->SetupAttachment(RootComponent);
    Trigger->InitBoxExtent(FVector(80.f));                 // ajusta tamaño a gusto
    Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Trigger->SetCollisionObjectType(ECC_WorldDynamic);
    Trigger->SetCollisionResponseToAllChannels(ECR_Ignore);
    Trigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    Trigger->SetGenerateOverlapEvents(true);
}

void ACajaFuerte::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    UStaticMesh* target = (bModoChasis && MeshChasis) ? MeshChasis : MeshCompleta;
    if (MeshHerramienta && target)
    {
        MeshHerramienta->SetStaticMesh(target);
        MeshHerramienta->SetWorldScale3D(EscalaComun);    
    }
    if (Trigger) Trigger->SetWorldScale3D(FVector(1.f));  
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
    {
        // aqui tu logica al entrar el Pawn (abrir UI, reproducir sonido, etc.)
        UE_LOG(LogTemp, Log, TEXT("CajaFuerte: begin overlap con %s"), *P->GetName());
    }
}

void ACajaFuerte::NotifyActorEndOverlap(AActor* OtherActor)
{
    Super::NotifyActorEndOverlap(OtherActor);

    if (APawn* P = Cast<APawn>(OtherActor))
    {
        // aqui tu logica al salir el Pawn (cerrar UI, etc.)
        UE_LOG(LogTemp, Log, TEXT("CajaFuerte: end overlap con %s"), *P->GetName());
    }
}
