// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GAME_TERRO_WGJCharacter.h"
#include "Engine/Engine.h"
#include "Monedas.generated.h"

UCLASS()
class GAME_TERRO_WGJ_API AMonedas : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonedas();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Mesh component for the monedas
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monedas Components")
	UStaticMeshComponent* MonedasMesh;

	// Trigger sphere for player detection
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monedas Components")
	USphereComponent* TriggerSphere;

	// Levitation variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Levitation")
	float LevitationHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Levitation")
	float LevitationSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
	float DetectionRadius;

	// Private variables for levitation effect
	FVector InitialLocation;
	float TimeElapsed;
	bool bIsCollected;

	// Trigger functions
	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
