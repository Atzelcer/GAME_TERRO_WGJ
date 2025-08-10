// Fill out your copyright notice in the Description page of Project Settings.


#include "Reja.h"

AReja::AReja()
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> HoeMesh(TEXT("StaticMesh'/Game/OldCatacombs/Meshes/SM_Door_A.SM_Door_A'"));
	if (HoeMesh.Succeeded())
	{
		MeshHerramienta->SetStaticMesh(HoeMesh.Object);
	}
	Identificador = TEXT("Reja");
}
