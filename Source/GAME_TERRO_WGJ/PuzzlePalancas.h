// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzlePalancas.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class GAME_TERRO_WGJ_API APuzzlePalancas : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APuzzlePalancas();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    USceneComponent* EscenaRaiz;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    USkeletalMeshComponent* Palanca1;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    USkeletalMeshComponent* Palanca2;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    USkeletalMeshComponent* Palanca3;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UBoxComponent* Trigger1;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UBoxComponent* Trigger2;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UBoxComponent* Trigger3;


    UPROPERTY(EditAnywhere, Category = "Audio")
    USoundBase* SonidoPalanca;

    // Orden correcto de palancas
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> OrdenCorrecto;

    // Secuencia actual
    TArray<int32> OrdenJugador;

  
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AActor> ObjetoRecompensa; // Clase del cenicero

    UPROPERTY(EditAnywhere, Category = "Animaciones")
    UAnimSequence* AnimacionPalanca;


private:

    UFUNCTION()
    void OnOverlapPalanca1(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
        const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapPalanca2(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
        const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapPalanca3(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
        const FHitResult& SweepResult);

    void VerificarSecuencia();
    void ReiniciarPuzzle();
    void CompletarPuzzle();

};
