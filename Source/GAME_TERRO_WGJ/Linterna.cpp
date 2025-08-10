// Fill out your copyright notice in the Description page of Project Settings.


#include "Linterna.h"

ALinterna::ALinterna()
{
	PrimaryActorTick.bCanEverTick = true;
	// Set the mesh for the lantern
	static ConstructorHelpers::FObjectFinder<UStaticMesh> LanternMesh(TEXT("StaticMesh'/Game/herramientasMega/Meshes/06_Lamp/SM_Lamp.SM_Lamp'"));
	if (LanternMesh.Succeeded())
	{
		MeshHerramienta->SetStaticMesh(LanternMesh.Object);
	}

	Identificador = TEXT("Linterna");
}
