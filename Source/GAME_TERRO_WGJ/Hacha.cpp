// Fill out your copyright notice in the Description page of Project Settings.


#include "Hacha.h"

AHacha::AHacha()
{
	PrimaryActorTick.bCanEverTick = true;
	// Set the mesh for the axe
	static ConstructorHelpers::FObjectFinder<UStaticMesh> AxeMesh(TEXT("StaticMesh'/Game/herramientasMega/hacha/Firefighter_Axe/054ac4a1162f4430a69b7dc80e679dad.054ac4a1162f4430a69b7dc80e679dad'"));
	if (AxeMesh.Succeeded())
	{
		MeshHerramienta->SetStaticMesh(AxeMesh.Object);
	}
	Identificador = TEXT("Hacha");
}
