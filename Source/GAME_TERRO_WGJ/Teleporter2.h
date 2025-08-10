// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GAME_TERRO_WGJCharacter.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Teleporter2.generated.h"

UCLASS()
class GAME_TERRO_WGJ_API ATeleporter2 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeleporter2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Main mesh component (invisible)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Teleporter Components")
	UStaticMeshComponent* TeleporterMesh;

	// Trigger sphere for player detection
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Teleporter Components")
	USphereComponent* TriggerSphere;

	// Particle system for visual effect
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Teleporter Components")
	UParticleSystemComponent* TeleportEffect;

	// Teleporter configuration
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleporter Settings")
	FString DestinationMapName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleporter Settings")
	float TriggerRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleporter Settings")
	float TeleportDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleporter Settings")
	bool bRequiresInteraction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleporter Settings")
	bool bIsActive;

	// Private variables
	bool bPlayerInRange;
	bool bTeleportInProgress;
	float TeleportTimer;

	// Trigger functions
	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

	// Teleport functions
	UFUNCTION(BlueprintCallable, Category = "Teleporter")
	void StartTeleport();

	UFUNCTION(BlueprintCallable, Category = "Teleporter")
	void ExecuteTeleport();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Public interaction function
	UFUNCTION(BlueprintCallable, Category = "Teleporter")
	void ActivateTeleporter();

};
