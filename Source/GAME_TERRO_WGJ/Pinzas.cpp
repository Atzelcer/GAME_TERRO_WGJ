// Fill out your copyright notice in the Description page of Project Settings.


#include "Pinzas.h"

// Sets default values
APinzas::APinzas()
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MallaPinzas(TEXT("SkeletalMesh'/Game/Decoraciones/Herramientas/Pinzas/bolt_cutter.bolt_cutter'"));
	if (MallaPinzas.Succeeded())
	{
		Mesh->SetSkeletalMesh(MallaPinzas.Object);
		Mesh->SetWorldScale3D(FVector(0.4, 0.4, 0.4));
	/*	Mesh->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));*/
	}
	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	Collision->SetupAttachment(RootComponent);
	Collision->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Collision->OnComponentBeginOverlap.AddDynamic(this, &APinzas::OnOverlapBegin);
}

// Called when the game starts or when spawned
void APinzas::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APinzas::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(FRotator(0, 60 * DeltaTime, 0));
}

void APinzas::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AGAME_TERRO_WGJCharacter* Player = Cast<AGAME_TERRO_WGJCharacter>(OtherActor))
	{
		Destroy();
	}


}

