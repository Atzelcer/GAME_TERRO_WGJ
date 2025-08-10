// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GAME_TERRO_WGJCharacter.h"
#include "Components/BoxComponent.h"
#include "Palanca.generated.h"

UCLASS()
class GAME_TERRO_WGJ_API APalanca : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APalanca();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Collision;

	AGAME_TERRO_WGJCharacter* Jugador;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};



