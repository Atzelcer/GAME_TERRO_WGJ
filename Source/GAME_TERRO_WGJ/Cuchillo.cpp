// Fill out your copyright notice in the Description page of Project Settings.


#include "Cuchillo.h"

ACuchillo::ACuchillo()
{
	PrimaryActorTick.bCanEverTick = true;
	// Set the mesh for the knife
	static ConstructorHelpers::FObjectFinder<UStaticMesh> KnifeMesh(TEXT("StaticMesh'/Game/BrutalButcher/Meshes/SM_Cleaver.SM_Cleaver'"));
	if (KnifeMesh.Succeeded())
	{
		MeshHerramienta->SetStaticMesh(KnifeMesh.Object);
	}
	Identificador = TEXT("Cuchillo");
}
