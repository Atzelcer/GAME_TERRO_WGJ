// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Calderon.h"
#include "Lena.generated.h"

UCLASS()
class GAME_TERRO_WGJ_API ALena : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALena();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    
    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Puzzle")
    ACalderon* RefCalderon;

    // Mesh de la leña
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
     USkeletalMeshComponent* Mesh;

     UPROPERTY(EditAnywhere, Category = "Audio")
     USoundBase* SonidoAgarrar;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class USphereComponent* PickupTrigger;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
        const FHitResult& SweepResult);

    bool bRecogida;

    void Recoger();

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLenaRecogida, ALena*, Lena);
    UPROPERTY(BlueprintAssignable, Category = "Pickup")
    FOnLenaRecogida OnLenaRecogida;
};
