// Fill out your copyright notice in the Description page of Project Settings.


#include "OB_Altar_Codicia.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"
#include "GAME_TERRO_WGJCharacter.h"

// Sets default values
AOB_Altar_Codicia::AOB_Altar_Codicia()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false; // No need for tick

	// Initialize state
	bHasBeenTriggered = false;
	TriggerRadius = 300.0f;

	// Create trigger sphere component
	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
	RootComponent = TriggerSphere;
	TriggerSphere->SetSphereRadius(TriggerRadius);
	TriggerSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// Create invisible mesh component
	AltarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AltarMesh"));
	AltarMesh->SetupAttachment(RootComponent);
	
	// Load basic mesh but make it invisible
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (MeshAsset.Succeeded())
	{
		AltarMesh->SetStaticMesh(MeshAsset.Object);
		AltarMesh->SetWorldScale3D(FVector(0.1f, 0.1f, 0.1f)); // Very small
		AltarMesh->SetVisibility(false); // Make invisible
		AltarMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// Create particle system component
	ParticleEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleEffect"));
	ParticleEffect->SetupAttachment(RootComponent);
	
	// Load codicia particle system
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("ParticleSystem'/Game/MagicCircles2/ParticleSystems/PS_MagicCircle_14.PS_MagicCircle_14'"));
	if (ParticleAsset.Succeeded())
	{
		ParticleEffect->SetTemplate(ParticleAsset.Object);
		ParticleEffect->SetAutoActivate(false); // Don't auto-activate
	}

	// Bind overlap event
	TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &AOB_Altar_Codicia::OnTriggerBeginOverlap);
}

// Called when the game starts or when spawned
void AOB_Altar_Codicia::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOB_Altar_Codicia::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AOB_Altar_Codicia::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if it's the player and hasn't been triggered yet
	if (OtherActor && OtherActor->IsA<AGAME_TERRO_WGJCharacter>() && !bHasBeenTriggered)
	{
		bHasBeenTriggered = true;
		
		// Activate particle effect
		if (ParticleEffect)
		{
			ParticleEffect->Activate();
		}
		
		// Schedule destruction after particle duration (adjust time as needed)
		FTimerHandle DestroyTimer;
		GetWorld()->GetTimerManager().SetTimer(DestroyTimer, [this]()
		{
			Destroy();
		}, 5.0f, false); // Destroy after 5 seconds
	}
}

