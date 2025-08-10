// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Calderon.generated.h"

UCLASS()
class GAME_TERRO_WGJ_API ACalderon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACalderon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UStaticMeshComponent* Mesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UBoxComponent* Trigger;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle")
    int32 LenasNecesarias;


    int32 LenasEntregadas;

    UPROPERTY(EditAnywhere, Category = "Recompensa")
    TSubclassOf<AActor> ClaseAnillo;


    UPROPERTY(EditAnywhere, Category = "Audio")
    USoundBase* SonidoFuego;

    UPROPERTY(EditAnywhere, Category = "Audio")
    USoundBase* SonidoAmbiente;

    UFUNCTION()
    void OnTriggerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
        const FHitResult& SweepResult);

    void EntregarLena();

    void ActivarTrigger();

private:
    void CompletarPuzzle();
};