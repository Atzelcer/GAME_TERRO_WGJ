// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleActivador.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UTexture2D;

UCLASS()
class GAME_TERRO_WGJ_API APuzzleActivador : public AActor
{
	GENERATED_BODY()
	
public:
    APuzzleActivador();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
    virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

    UFUNCTION(BlueprintCallable, Category = "Puzzle")
    void SetContenido(UTexture2D* InTexture, int32 InDigit);

    // Componentes
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle|Components")
    UStaticMeshComponent* MeshComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle|Components")
    UBoxComponent* Trigger;

    // Datos que muestra el widget
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle")
    UTexture2D* Imagen = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle")
    int32 Numero = 0;
};