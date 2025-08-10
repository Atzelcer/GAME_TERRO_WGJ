// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePalancas.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APuzzlePalancas::APuzzlePalancas()
{
    PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FClassFinder<AActor> CeniceroClass(TEXT("Class'/Script/GAME_TERRO_WGJ.Cenicero'"));
    if (CeniceroClass.Succeeded())
    {
        ObjetoRecompensa = CeniceroClass.Class;
    }

    static ConstructorHelpers::FObjectFinder<USoundBase> Sonido(TEXT("SoundWave'/Game/Decoraciones/Sonido/bajandoPalanca.bajandoPalanca'"));
    if (Sonido.Succeeded())
    {
        SonidoPalanca = Sonido.Object;
    }
    //Animacion
    static ConstructorHelpers::FObjectFinder<UAnimSequence> Anim(TEXT("SkeletalMesh'/Game/Decoraciones/Herramientas/Palanca/Lever_Export_With_Animation.Lever_Export_With_Animation_polySurface10'"));
    if (Anim.Succeeded())
    {
        AnimacionPalanca = Anim.Object;
        Palanca1->SetAnimation(AnimacionPalanca);
        Palanca2->SetAnimation(AnimacionPalanca);
        Palanca3->SetAnimation(AnimacionPalanca);
    }

    EscenaRaiz = CreateDefaultSubobject<USceneComponent>(TEXT("EscenaRaiz"));
    RootComponent = EscenaRaiz;

    Palanca1 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Palanca1"));
    Palanca1->SetupAttachment(RootComponent);

    Palanca2 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Palanca2"));
    Palanca2->SetupAttachment(RootComponent);

    Palanca3 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Palanca3"));
    Palanca3->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> PalancaMesh(TEXT("SkeletalMesh'/Game/Decoraciones/Herramientas/Palanca/Lever_Export_With_Animation.Lever_Export_With_Animation_polySurface10'"));
    if (PalancaMesh.Succeeded())
    {
        Palanca1->SetSkeletalMesh(PalancaMesh.Object);
        Palanca2->SetSkeletalMesh(PalancaMesh.Object);
        Palanca3->SetSkeletalMesh(PalancaMesh.Object);
    }

    Palanca1->SetRelativeLocation(FVector(0.f, 0.f, 0.f));   
    Palanca2->SetRelativeLocation(FVector(100.f, 0.f, 0.f)); 
    Palanca3->SetRelativeLocation(FVector(200, 0.f, 0.f));

    Palanca1->SetWorldScale3D(FVector(2));
    Palanca2->SetWorldScale3D(FVector(2));
    Palanca3->SetWorldScale3D(FVector(2));


    Trigger1 = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger1"));
    Trigger1->SetupAttachment(Palanca1);
    Trigger1->SetBoxExtent(FVector(15,15, 35));


    Trigger2 = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger2"));
    Trigger2->SetupAttachment(Palanca2);
    Trigger2->SetBoxExtent(FVector(15, 15, 35));

    Trigger3 = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger3"));
    Trigger3->SetupAttachment(Palanca3);
    Trigger3->SetBoxExtent(FVector(15, 15, 35));

 
    OrdenCorrecto = { 2, 1, 3 };
}

// Called when the game starts or when spawned
void APuzzlePalancas::BeginPlay()
{
	Super::BeginPlay();
	
    Trigger1->OnComponentBeginOverlap.AddDynamic(this, &APuzzlePalancas::OnOverlapPalanca1);
    Trigger2->OnComponentBeginOverlap.AddDynamic(this, &APuzzlePalancas::OnOverlapPalanca2);
    Trigger3->OnComponentBeginOverlap.AddDynamic(this, &APuzzlePalancas::OnOverlapPalanca3);
}

// Called every frame
void APuzzlePalancas::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void APuzzlePalancas::OnOverlapPalanca1(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult& SweepResult)
{
    OrdenJugador.Add(1);
    VerificarSecuencia();
}

void APuzzlePalancas::OnOverlapPalanca2(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult& SweepResult)
{
    OrdenJugador.Add(2);
    VerificarSecuencia();
}

void APuzzlePalancas::OnOverlapPalanca3(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult& SweepResult)
{
    OrdenJugador.Add(3);
    VerificarSecuencia();
}

void APuzzlePalancas::VerificarSecuencia()
{
    int32 Index = OrdenJugador.Num() - 1;

    if (AnimacionPalanca)
    {
        Palanca1->PlayAnimation(AnimacionPalanca, false);
    }

    if (SonidoPalanca)
    {
        UGameplayStatics::PlaySoundAtLocation(this, SonidoPalanca, GetActorLocation());
    }
   
    if (Index >= OrdenCorrecto.Num() || OrdenJugador[Index] != OrdenCorrecto[Index])
    {
        ReiniciarPuzzle();
        return;
    }

    // Si ya está completa la secuencia
    if (OrdenJugador.Num() == OrdenCorrecto.Num())
    {
        CompletarPuzzle();
    }
}

void APuzzlePalancas::ReiniciarPuzzle()
{
    OrdenJugador.Empty();
    GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("Secuencia incorrecta. Reiniciando..."));
}

void APuzzlePalancas::CompletarPuzzle()
{
    GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("Puzzle completado!"));

    if (ObjetoRecompensa)
    {
        FActorSpawnParameters SpawnParams;
        FVector SpawnLocation = FVector(500, 1210, 160);
        FRotator SpawnRotation = FRotator::ZeroRotator;
        GetWorld()->SpawnActor<AActor>(ObjetoRecompensa, SpawnLocation, SpawnRotation, SpawnParams);
    }
}

