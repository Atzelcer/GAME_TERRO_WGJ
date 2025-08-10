// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "OB_Altar_Muerte.generated.h"

UCLASS()
class GAME_TERRO_WGJ_API AOB_Altar_Muerte : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOB_Altar_Muerte();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Altar Components")
	UStaticMeshComponent* AltarMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Altar Components")
	USphereComponent* TriggerSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Altar Components")
	UParticleSystemComponent* ParticleEffect;

	// Trigger settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Altar Settings")
	float TriggerRadius;

	UPROPERTY(BlueprintReadOnly, Category = "Altar State")
	bool bHasBeenTriggered;

	// Trigger function
	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
