// Fill out your copyright notice in the Description page of Project Settings.


#include "Flor.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SphereComponent.h"

// Sets default values
AFlor::AFlor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create and set up the mesh component
	FlorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlorMesh"));
	RootComponent = FlorMesh;

	// Create trigger sphere for player detection
	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
	TriggerSphere->SetupAttachment(RootComponent);
	
	// Set up trigger sphere properties
	DetectionRadius = 65.0f;
	TriggerSphere->SetSphereRadius(DetectionRadius);
	TriggerSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	// Bind trigger event
	TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &AFlor::OnTriggerBeginOverlap);

	// Load the flor mesh from OBJECTS_SCENE3
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/OBJECTS_SCENE3/floweroflife/source/FlorNEW.FlorNEW'"));
	if (MeshAsset.Succeeded())
	{
		FlorMesh->SetStaticMesh(MeshAsset.Object);
		
		// Scale and position for flor
		FlorMesh->SetWorldScale3D(FVector(3.0f, 3.0f, 3.0f));
		FlorMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}

	// Initialize levitation variables
	LevitationHeight = 15.0f;
	LevitationSpeed = 1.5f;
	TimeElapsed = 0.0f;
	bIsCollected = false;
}

// Called when the game starts or when spawned
void AFlor::BeginPlay()
{
	Super::BeginPlay();
	
	// Store the initial location for levitation calculation
	InitialLocation = GetActorLocation();
}

// Called every frame
void AFlor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Only update levitation if not collected
	if (!bIsCollected)
	{
		// Update time for levitation effect
		TimeElapsed += DeltaTime;

		// Calculate new Z position using sine wave for smooth up/down movement
		float NewZ = InitialLocation.Z + (FMath::Sin(TimeElapsed * LevitationSpeed) * LevitationHeight);
		
		// Apply the new location
		FVector NewLocation = FVector(InitialLocation.X, InitialLocation.Y, NewZ);
		SetActorLocation(NewLocation);
	}
}

// Trigger function when player enters detection range
void AFlor::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if the other actor is the player character
	if (OtherActor && OtherActor->IsA<AGAME_TERRO_WGJCharacter>() && !bIsCollected)
	{
		bIsCollected = true;

		// Destroy the object after a short delay for visual feedback
		SetLifeSpan(0.1f);
	}
}

