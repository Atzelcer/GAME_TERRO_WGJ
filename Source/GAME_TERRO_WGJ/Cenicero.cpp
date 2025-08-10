// Fill out your copyright notice in the Description page of Project Settings.


#include "Cenicero.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
ACenicero::ACenicero()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MallaCenicero(TEXT("StaticMesh'/Game/Decoraciones/Articulos_Pueblo/Cenicero/Medieval_Jug_lowpoly_FBX.Medieval_Jug_lowpoly_FBX'"));
	if (MallaCenicero.Succeeded())
	{
		Mesh->SetSkeletalMesh(MallaCenicero.Object);
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
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ACenicero::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ACenicero::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACenicero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	AddActorLocalRotation(FRotator(0, 60 * DeltaTime, 0));
}

void ACenicero::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

