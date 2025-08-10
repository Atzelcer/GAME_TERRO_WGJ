// Fill out your copyright notice in the Description page of Project Settings.


#include "HerramientasP.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AHerramientasP::AHerramientasP()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshHerramienta = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshHerramienta"));
	RootComponent = MeshHerramienta;

	Identificador = TEXT("Herramienta");
}

// Called when the game starts or when spawned
void AHerramientasP::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHerramientasP::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

