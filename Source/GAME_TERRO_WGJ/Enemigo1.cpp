// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemigo1.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

AEnemigo1::AEnemigo1()
{
	// Load BrutalButcher Skeletal Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(
		TEXT("SkeletalMesh'/Game/BrutalButcher/Meshes/SK_Butcher.SK_Butcher'")
	);
	if (MeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
		
		// Adjust mesh position and rotation for BrutalButcher
		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -88.0f));
		GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	}
}