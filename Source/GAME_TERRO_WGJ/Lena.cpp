// Fill out your copyright notice in the Description page of Project Settings.


#include "Lena.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALena::ALena()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> Lena(TEXT("SkeletalMesh'/Game/Decoraciones/Articulos_Pueblo/Lena/holzscheite.holzscheite'"));
    if (Lena.Succeeded())
    {
        Mesh->SetSkeletalMesh(Lena.Object);
        /*	Mesh->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));*/
    }

    static ConstructorHelpers::FObjectFinder<USoundBase> Sonido(TEXT("SoundWave'/Game/Decoraciones/Sonido/agarrarObjeto.agarrarObjeto'"));
    if (Sonido.Succeeded())
    {
        SonidoAgarrar = Sonido.Object;
    }


    PickupTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("PickupTrigger"));
    PickupTrigger->SetupAttachment(RootComponent);
    PickupTrigger->SetSphereRadius(100.f);

    PickupTrigger->OnComponentBeginOverlap.AddDynamic(this, &ALena::OnOverlapBegin);

    bRecogida = false;
}

// Called when the game starts or when spawned
void ALena::BeginPlay()
{
	Super::BeginPlay();
    if (!RefCalderon)
    {
        TArray<AActor*> Encontrados;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACalderon::StaticClass(), Encontrados);

        if (Encontrados.Num() > 0)
        {
            RefCalderon = Cast<ACalderon>(Encontrados[0]);
        }
    }
}

// Called every frame
void ALena::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ALena::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (bRecogida) return;

    if (OtherActor && OtherActor != this)
    {
        // Aquí podrías comprobar que OtherActor es el jugador, por ejemplo
        Recoger();

        RefCalderon->EntregarLena();

        if (SonidoAgarrar)
        {
            UGameplayStatics::PlaySoundAtLocation(this, SonidoAgarrar, GetActorLocation());
        }


    }
}

void ALena::Recoger()
{
    if (bRecogida) return;

    bRecogida = true;

    // Desactivar la colisión y esconder la malla para simular que fue recogida
    Mesh->SetVisibility(false);
    Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    PickupTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // Notificar a quien esté escuchando que esta leña fue recogida
    OnLenaRecogida.Broadcast(this);
}
