// Fill out your copyright notice in the Description page of Project Settings.


#include "Pala.h"

APala::APala()
{
	PrimaryActorTick.bCanEverTick = true;
	// Set the mesh for the knife
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PalaMesh(TEXT("StaticMesh'/Game/packComplemento/Models/Props/SM_Shovel.SM_Shovel'"));
	if (PalaMesh.Succeeded())
	{
		MeshHerramienta->SetStaticMesh(PalaMesh.Object);
	}
	Identificador = TEXT("Pala");
}
