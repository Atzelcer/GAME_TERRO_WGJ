// Fill out your copyright notice in the Description page of Project Settings.


#include "Teleporter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATeleporter::ATeleporter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create and set up the mesh component (invisible)
	TeleporterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TeleporterMesh"));
	RootComponent = TeleporterMesh;

	// Load invisible cube mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	if (MeshAsset.Succeeded())
	{
		TeleporterMesh->SetStaticMesh(MeshAsset.Object);
		TeleporterMesh->SetWorldScale3D(FVector(2.0f, 2.0f, 0.1f)); // Flat platform
		TeleporterMesh->SetVisibility(false); // Make invisible
		TeleporterMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); // No collision
	}

	// Create trigger sphere for player detection
	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
	TriggerSphere->SetupAttachment(RootComponent);
	
	// Set up trigger sphere properties
	TriggerRadius = 200.0f;
	TriggerSphere->SetSphereRadius(TriggerRadius);
	TriggerSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	// Bind trigger events
	TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &ATeleporter::OnTriggerBeginOverlap);
	TriggerSphere->OnComponentEndOverlap.AddDynamic(this, &ATeleporter::OnTriggerEndOverlap);

	// Create particle system component
	TeleportEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TeleportEffect"));
	TeleportEffect->SetupAttachment(RootComponent);
	
	// Load a magical particle system (you can change this path to your preferred effect)
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("ParticleSystem'/Game/MagicCircles2/ParticleSystems/PS_MagicCircle_18.PS_MagicCircle_18'"));
	if (ParticleAsset.Succeeded())
	{
		TeleportEffect->SetTemplate(ParticleAsset.Object);
		TeleportEffect->SetAutoActivate(true);
	}

	// Initialize default settings
	TeleporterType = ETeleporterType::BosqueToAldea; // Default to first in sequence
	DestinationMapName = TEXT("/Game/OldVillage/Levels/Old_Village_Night"); // Default to Aldea
	TeleportDelay = 2.0f; // 2 second delay for teleport
	bRequiresInteraction = false; // Auto-teleport when entering trigger
	bIsActive = true;
	
	// Configure destination based on type
	ConfigureDestinationByType();
	
	// Initialize state variables
	bPlayerInRange = false;
	bTeleportInProgress = false;
	TeleportTimer = 0.0f;
}

// Called when the game starts or when spawned
void ATeleporter::BeginPlay()
{
	Super::BeginPlay();
	
	// Configure destination based on type (in case it was changed in editor)
	ConfigureDestinationByType();
	
	// Ensure the teleporter is active and ready
	if (GEngine && bIsActive)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, 
			FString::Printf(TEXT("Teleporter active - Destination: %s"), *DestinationMapName));
	}
}

// Called every frame
void ATeleporter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Handle teleport timer
	if (bTeleportInProgress)
	{
		TeleportTimer += DeltaTime;
		
		// Show countdown to player
		if (GEngine)
		{
			float TimeLeft = TeleportDelay - TeleportTimer;
			if (TimeLeft > 0)
			{
				GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Yellow, 
					FString::Printf(TEXT("Teleporting in %.1f seconds..."), TimeLeft));
			}
		}
		
		// Execute teleport when timer reaches delay
		if (TeleportTimer >= TeleportDelay)
		{
			ExecuteTeleport();
		}
	}
}

// Trigger function when player enters
void ATeleporter::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if the other actor is the player character
	if (OtherActor && OtherActor->IsA<AGAME_TERRO_WGJCharacter>() && bIsActive)
	{
		bPlayerInRange = true;
		
		if (GEngine)
		{
			if (bRequiresInteraction)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Press E to use Teleporter"));
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Teleporter activated!"));
				StartTeleport();
			}
		}
	}
}

// Trigger function when player exits
void ATeleporter::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	// Check if the other actor is the player character
	if (OtherActor && OtherActor->IsA<AGAME_TERRO_WGJCharacter>())
	{
		bPlayerInRange = false;
		
		// Cancel teleport if player leaves area
		if (bTeleportInProgress)
		{
			bTeleportInProgress = false;
			TeleportTimer = 0.0f;
			
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Teleport cancelled - Player left area"));
			}
		}
	}
}

// Start teleport process
void ATeleporter::StartTeleport()
{
	if (!bIsActive || bTeleportInProgress)
	{
		return;
	}
	
	bTeleportInProgress = true;
	TeleportTimer = 0.0f;
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, 
			FString::Printf(TEXT("Teleporting to: %s"), *DestinationMapName));
	}
}

// Execute the actual teleport
void ATeleporter::ExecuteTeleport()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, TEXT("Teleporting now!"));
	}
	
	// Try different methods to open the level
	if (!DestinationMapName.IsEmpty())
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, 
				FString::Printf(TEXT("Attempting to load map: %s"), *DestinationMapName));
		}
		
		// Method 1: Direct path (works best for Unreal maps)
		UGameplayStatics::OpenLevel(this, FName(*DestinationMapName));
		
		// Alternative method if the first doesn't work:
		// UGameplayStatics::OpenLevel(GetWorld(), FName(*DestinationMapName));
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("ERROR: DestinationMapName is empty!"));
		}
	}
	
	// Reset state
	bTeleportInProgress = false;
	TeleportTimer = 0.0f;
}

// Public function to activate teleporter manually
void ATeleporter::ActivateTeleporter()
{
	if (bPlayerInRange && bIsActive)
	{
		StartTeleport();
	}
}

// Configure destination based on teleporter type
void ATeleporter::ConfigureDestinationByType()
{
	switch (TeleporterType)
	{
	case ETeleporterType::BosqueToAldea:
		DestinationMapName = TEXT("/Game/OldVillage/Levels/Old_Village_Night");
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Teleporter configured: Bosque → Aldea"));
		}
		break;
		
	case ETeleporterType::AldeaToCatacumbas:
		DestinationMapName = TEXT("/Game/OldCatacombs/Maps/Demo");
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Teleporter configured: Aldea → Catacumbas"));
		}
		break;
		
	case ETeleporterType::CatacumbasToBosque:
		// Assuming you have a forest/bosque map - replace with your actual forest map path
		DestinationMapName = TEXT("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap"); // Change this to your forest map
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, TEXT("Teleporter configured: Catacumbas → Bosque"));
		}
		break;
		
	case ETeleporterType::Custom:
	default:
		// Keep the current DestinationMapName (manually set)
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Purple, TEXT("Teleporter configured: Custom destination"));
		}
		break;
	}
}

