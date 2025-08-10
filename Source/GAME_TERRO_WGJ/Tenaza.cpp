// Fill out your copyright notice in the Description page of Project Settings.


#include "Tenaza.h"

ATenaza::ATenaza()
{
	PrimaryActorTick.bCanEverTick = true;
	// Set the mesh for the tongs
	static ConstructorHelpers::FObjectFinder<UStaticMesh> TongsMesh(TEXT("StaticMesh'/Game/herramientasMega/Meshes/13_Pliers/SM_Pliers.SM_Pliers'"));
	if (TongsMesh.Succeeded())
	{
		MeshHerramienta->SetStaticMesh(TongsMesh.Object);
	}
	Identificador = TEXT("Tenaza");
}
