// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HerramientasP.h"
#include "CajaFuerte.generated.h"


class UStaticMesh;
class UBoxComponent;

UCLASS()
class GAME_TERRO_WGJ_API ACajaFuerte : public AHerramientasP
{
	GENERATED_BODY()
	

public:
    ACajaFuerte();
    virtual void OnConstruction(const FTransform& Transform) override;
    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
    virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

    UFUNCTION(BlueprintCallable, Category = "Safe")
    void SetModoChasis(bool bNew);

    UFUNCTION(BlueprintCallable, Category = "Safe")
    void SetEscalaComun(float s);

    UFUNCTION(BlueprintCallable, Category = "Safe")
    void DesactivarColision();          // desactiva solo el trigger

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Safe")
    UStaticMesh* MeshCompleta = nullptr;

    UPROPERTY(EditDefaultsOnly, Category = "Safe")
    UStaticMesh* MeshChasis = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Safe")
    bool bModoChasis = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Safe")
    FVector EscalaComun = FVector(1.f);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Safe|Collision")
    UBoxComponent* Trigger = nullptr;
};