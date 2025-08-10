// Fill out your copyright notice in the Description page of Project Settings.


#include "Aceite.h"

// Sets default values
AAceite::AAceite()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MallaAceite(TEXT("SkeletalMesh'/Game/Decoraciones/Herramientas/Aceite/oil_can_SF.oil_can_SF'"));
	if (MallaAceite.Succeeded())
	{
		Mesh->SetSkeletalMesh(MallaAceite.Object);
		/*	Mesh->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));*/
	}
	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	Collision->SetupAttachment(RootComponent);
	Collision->SetBoxExtent(FVector(10, 10, 10));
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AAceite::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AAceite::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAceite::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(FRotator(0, 60 * DeltaTime, 0));
}


void AAceite::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AGAME_TERRO_WGJCharacter* Player = Cast<AGAME_TERRO_WGJCharacter>(OtherActor))
	{
		Destroy();
	}


}

