// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HerramientasP.h"
#include "Tenaza.generated.h"

class USphereComponent;
class USoundBase;
class AReja;

UCLASS()
class GAME_TERRO_WGJ_API ATenaza : public AHerramientasP
{
	GENERATED_BODY()

public:

	ATenaza();

	virtual void BeginPlay() override;

protected:
	// Trigger para recoger la tenaza
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
	USphereComponent* Trigger = nullptr;

	// Sonido al recoger
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundBase* SndAgarrar = nullptr;

	// Reja a activar cuando se recoge la tenaza (asígnala en el editor)
	UPROPERTY(EditInstanceOnly, Category = "Reja")
	AReja* RejaObjetivo = nullptr;

	// Overlap a nivel de actor
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};