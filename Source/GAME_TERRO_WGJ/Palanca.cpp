// Fill out your copyright notice in the Description page of Project Settings.


#include "Palanca.h"

// Sets default values
APalanca::APalanca()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MallaPalanca(TEXT("SkeletalMesh'/Game/Decoraciones/Herramientas/Palanca/Lever_Export_With_Animation.Lever_Export_With_Animation_polySurface10'"));
	if (MallaPalanca.Succeeded())
	{
		Mesh->SetSkeletalMesh(MallaPalanca.Object);
		/*	Mesh->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));*/
	}
	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	Collision->SetupAttachment(RootComponent);
	Collision->SetBoxExtent(FVector(10, 10, 10));
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Collision->OnComponentBeginOverlap.AddDynamic(this, &APalanca::OnOverlapBegin);
}

// Called when the game starts or when spawned
void APalanca::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APalanca::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorLocalRotation(FRotator(0, 60 * DeltaTime, 0));
}
void APalanca::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AGAME_TERRO_WGJCharacter* Player = Cast<AGAME_TERRO_WGJCharacter>(OtherActor))
	{
		Destroy();
	}


}


