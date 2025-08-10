// Fill out your copyright notice in the Description page of Project Settings.


#include "CajaFuerte.h"

ACajaFuerte::ACajaFuerte()
{
	PrimaryActorTick.bCanEverTick = true;
	// Set the mesh for the safe
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SafeMesh(TEXT("StaticMesh'/Game/packComplemento/Models/Props/SM_Safe.SM_Safe'"));
	if (SafeMesh.Succeeded())
	{
		MeshHerramienta->SetStaticMesh(SafeMesh.Object);
	}
	Identificador = TEXT("CajaFuerte");

	// para static mesh secundario 
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SafeMesh2(TEXT("StaticMesh'/Game/packComplemento/Models/Props/SM_Safe_Chassis.SM_Safe_Chassis'"));

}
