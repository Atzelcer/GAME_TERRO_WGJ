// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HerramientasP.h"
#include "Reja.generated.h"

class UAudioComponent;
class USoundBase;

UCLASS()
class GAME_TERRO_WGJ_API AReja : public AHerramientasP
{
	GENERATED_BODY()
	
public:
	AReja();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "Reja")
	void SubirReja();

	UFUNCTION(BlueprintCallable, Category = "Reja")
	void ReproducirSonidoReja();

protected:
	UPROPERTY(EditAnywhere, Category = "Reja|Movimiento")
	float AlturaSubida = 500.f;           // cuánto sube en Z

	UPROPERTY(EditAnywhere, Category = "Reja|Movimiento")
	float DuracionSubida = 2.0f;          // en segundos

	UPROPERTY(EditAnywhere, Category = "Reja|Movimiento")
	float FactorEscalaZ = 0.85f;          // 0.85 = reduce 15% el Z

private:
	bool   bSubiendo = false;
	float  T0 = 0.f;
	FVector LocIni, LocFin;
	FVector ScaleIni, ScaleFin;

	// --- Audio ---
	UPROPERTY(VisibleAnywhere, Category = "Reja|Audio")
	UAudioComponent* AudioComp = nullptr;

	UPROPERTY(EditAnywhere, Category = "Reja|Audio")
	USoundBase* SndSubir = nullptr; 
};
