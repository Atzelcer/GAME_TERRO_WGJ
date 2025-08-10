// Fill out your copyright notice in the Description page of Project Settings.


#include "Calderon.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GAME_TERRO_WGJCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACalderon::ACalderon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> Calderon(TEXT("Blueprint'/Game/OldVillage/Meshes/BP_Cauldron_001.BP_Cauldron_001'"));
    if (Calderon.Succeeded())
    {
        Mesh->SetStaticMesh(Calderon.Object);
        /*	Mesh->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));*/
    }

    //Sonido
    static ConstructorHelpers::FObjectFinder<USoundBase> Sonido(TEXT("SoundWave'/Game/Decoraciones/Sonido/antorchaFuego.antorchaFuego'"));
    if (Sonido.Succeeded())
    {
        SonidoFuego = Sonido.Object;


    }
    static ConstructorHelpers::FObjectFinder<USoundBase> Ambiente(TEXT("SoundWave'/Game/Decoraciones/Sonido/Musica4.Musica4'"));
    if (Ambiente.Succeeded())
    {
        SonidoAmbiente = Ambiente.Object;


    }

    Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
    Trigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Trigger->SetGenerateOverlapEvents(false);
    Trigger->SetupAttachment(RootComponent);
    Trigger->SetBoxExtent(FVector(120.f, 120.f, 120.f));

    LenasNecesarias = 4;
    LenasEntregadas = 0;
}

// Called when the game starts or when spawned
void ACalderon::BeginPlay()
{
	Super::BeginPlay();
	
    Trigger->OnComponentBeginOverlap.AddDynamic(this, &ACalderon::OnTriggerOverlap);


    if (SonidoFuego)
    {
        UGameplayStatics::PlaySoundAtLocation(this, SonidoFuego, GetActorLocation());
    }

    if (SonidoAmbiente)
    {
        UGameplayStatics::PlaySoundAtLocation(this, SonidoAmbiente, GetActorLocation());
    }
}

// Called every frame
void ACalderon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACalderon::ActivarTrigger()
{
    Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Trigger->SetGenerateOverlapEvents(true);

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Trigger del calderon activado, ya puedes usarlo"));
}

void ACalderon::OnTriggerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult& SweepResult)
{
   
    if (OtherActor && OtherActor->IsA(AGAME_TERRO_WGJCharacter::StaticClass())) 
    {
        CompletarPuzzle();

       
        Trigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        Trigger->SetGenerateOverlapEvents(false);
    }
}

void ACalderon::EntregarLena()
{
    LenasEntregadas++;

    if (LenasEntregadas >= LenasNecesarias)
    {
        ActivarTrigger();
    }
}

void ACalderon::CompletarPuzzle()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Calderon completado, aparece el anillo!"));

    if (ClaseAnillo)
    {
        FVector SpawnLocation = FVector(2020, 2110, 50);
        FRotator SpawnRotation = FRotator(180, 0, 0);
        GetWorld()->SpawnActor<AActor>(ClaseAnillo, SpawnLocation, SpawnRotation);
    }
}
