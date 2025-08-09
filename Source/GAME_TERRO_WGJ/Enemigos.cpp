// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemigos.h"

// Sets default values
AEnemigos::AEnemigos()
{
	// Set this character to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemigos::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnemigos::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
