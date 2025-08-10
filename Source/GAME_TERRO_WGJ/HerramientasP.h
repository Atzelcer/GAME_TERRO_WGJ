// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HerramientasP.generated.h"

UCLASS()
class GAME_TERRO_WGJ_API AHerramientasP : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHerramientasP();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Herramienta")
	class UStaticMeshComponent* MeshHerramienta;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Herramienta")
	FName Identificador;
};