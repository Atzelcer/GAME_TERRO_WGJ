// Fill out your copyright notice in the Description page of Project Settings.


#include "Pergamino.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SphereComponent.h"

// Sets default values
APergamino::APergamino()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create and set up the mesh component
	PergaminoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PergaminoMesh"));
	RootComponent = PergaminoMesh;

	// Create trigger sphere for player detection
	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
	TriggerSphere->SetupAttachment(RootComponent);
	
	// Set up trigger sphere properties
	DetectionRadius = 35.0f;
	TriggerSphere->SetSphereRadius(DetectionRadius);
	TriggerSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	// Bind trigger event
	TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &APergamino::OnTriggerBeginOverlap);

	// Load the pergamino mesh from OBJECTS_SCENE3
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/OBJECTS_SCENE3/almas_pergamino/source/Pergamino.Pergamino'"));
	if (MeshAsset.Succeeded())
	{
		PergaminoMesh->SetStaticMesh(MeshAsset.Object);
		
		// Scale and position for pergamino
		PergaminoMesh->SetWorldScale3D(FVector(3.0f, 3.0f, 3.0f));
		PergaminoMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}

	// Initialize levitation variables
	LevitationHeight = 22.0f;
	LevitationSpeed = 1.4f;
	TimeElapsed = 0.0f;
	bIsCollected = false;
}

// Called when the game starts or when spawned
void APergamino::BeginPlay()
{
	Super::BeginPlay();
	
	// Store the initial location for levitation calculation
	InitialLocation = GetActorLocation();
}

// Called every frame
void APergamino::Tick(float DeltaTime)
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
void APergamino::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if the other actor is the player character
	if (OtherActor && OtherActor->IsA<AGAME_TERRO_WGJCharacter>() && !bIsCollected)
	{
		bIsCollected = true;
		
		// Log collection message
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Purple, TEXT("Pergamino collected!"));
		}

		// Destroy the object after a short delay for visual feedback
		SetLifeSpan(0.1f);
	}
}

