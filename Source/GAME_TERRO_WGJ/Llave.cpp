// Fill out your copyright notice in the Description page of Project Settings.


#include "Llave.h"

ALlave::ALlave()
{
	PrimaryActorTick.bCanEverTick = true;
	// Set the mesh for the knife
	static ConstructorHelpers::FObjectFinder<UStaticMesh> KnifeMesh(TEXT("StaticMesh'/Game/herramientasMega/Meshes/14_Screwdriver/SM_Screw_02.SM_Screw_02'"));
	if (KnifeMesh.Succeeded())
	{
		MeshHerramienta->SetStaticMesh(KnifeMesh.Object);
		
		MeshHerramienta->SetRelativeScale3D(FVector(6.5f, 6.5f, 6.5f));

	}
	Identificador = TEXT("Cuchillo");

}
