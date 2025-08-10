// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HistoriasActivador.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UNotascpp;

UCLASS()
class GAME_TERRO_WGJ_API AHistoriasActivador : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHistoriasActivador();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    // ? usamos notifies de actor
    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	void setHistoria(const FText& texto);

    UPROPERTY(VisibleAnywhere) UStaticMeshComponent* HistoriasActivity;
    UPROPERTY(VisibleAnywhere) UBoxComponent* Trigger;

    FText HistoriaMarkup;
};