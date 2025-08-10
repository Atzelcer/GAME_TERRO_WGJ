// Fill out your copyright notice in the Description page of Project Settings.


#include "Cuchillo.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACuchillo::ACuchillo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MallaCuchillo(TEXT("SkeletalMesh'/Game/Decoraciones/Articulos_Pueblo/Cuchillo/ps1-meyers-knife-a.ps1-meyers-knife-a'"));
	if (MallaCuchillo.Succeeded())
	{
		Mesh->SetSkeletalMesh(MallaCuchillo.Object);
		Mesh->SetWorldScale3D(FVector(0.2));
		Mesh->SetRelativeRotation(FRotator(0, 0, 0));
		/*	Mesh->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));*/
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> Sonido(TEXT("SoundWave'/Game/Decoraciones/Sonido/agarrarObjeto.agarrarObjeto'"));
	if (Sonido.Succeeded())
	{
		SonidoAgarrar = Sonido.Object;
	}

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	Collision->SetupAttachment(RootComponent);
	Collision->SetBoxExtent(FVector(10, 10, 10));
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ACuchillo::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ACuchillo::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACuchillo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorLocalRotation(FRotator(0, 60 * DeltaTime, 0));
}

void ACuchillo::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AGAME_TERRO_WGJCharacter* Player = Cast<AGAME_TERRO_WGJCharacter>(OtherActor))
	{
		Destroy();
		if (SonidoAgarrar)
		{
			UGameplayStatics::PlaySoundAtLocation(this, SonidoAgarrar, GetActorLocation());
		}

	}
}

