// Fill out your copyright notice in the Description page of Project Settings.

#include "Teleporter2.h"

// Sets default values
ATeleporter2::ATeleporter2()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create root component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Create teleporter mesh (invisible)
	TeleporterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TeleporterMesh"));
	TeleporterMesh->SetupAttachment(RootComponent);
	TeleporterMesh->SetVisibility(false);
	TeleporterMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Create trigger sphere
	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
	TriggerSphere->SetupAttachment(RootComponent);
	TriggerSphere->SetSphereRadius(150.0f);
	TriggerSphere->SetCollisionProfileName(TEXT("Trigger"));

	// Create particle effect
	TeleportEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TeleportEffect"));
	TeleportEffect->SetupAttachment(RootComponent);

	// Load particle system
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("ParticleSystem'/Game/MagicCircles2/ParticleSystems/PS_MagicCircle_17.PS_MagicCircle_17'"));
	if (ParticleAsset.Succeeded())
	{
		TeleportEffect->SetTemplate(ParticleAsset.Object);
	}

	// Default values
	DestinationMapName = TEXT("/Game/OldCatacombs/Maps/Demo"); // Aldea -> Catacumbas
	TriggerRadius = 150.0f;
	TeleportDelay = 2.0f;
	bRequiresInteraction = false;
	bIsActive = true;

	// Private variables
	bPlayerInRange = false;
	bTeleportInProgress = false;
	TeleportTimer = 0.0f;

	// Bind trigger events
	TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &ATeleporter2::OnTriggerBeginOverlap);
	TriggerSphere->OnComponentEndOverlap.AddDynamic(this, &ATeleporter2::OnTriggerEndOverlap);
}

// Called when the game starts or when spawned
void ATeleporter2::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATeleporter2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Handle teleport countdown
	if (bTeleportInProgress)
	{
		TeleportTimer -= DeltaTime;
		if (TeleportTimer <= 0.0f)
		{
			ExecuteTeleport();
		}
	}
}

void ATeleporter2::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsActive) return;

	AGAME_TERRO_WGJCharacter* Player = Cast<AGAME_TERRO_WGJCharacter>(OtherActor);
	if (Player)
	{
		bPlayerInRange = true;
		
		if (!bRequiresInteraction)
		{
			StartTeleport();
		}
	}
}

void ATeleporter2::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	AGAME_TERRO_WGJCharacter* Player = Cast<AGAME_TERRO_WGJCharacter>(OtherActor);
	if (Player)
	{
		bPlayerInRange = false;
		if (bTeleportInProgress)
		{
			bTeleportInProgress = false;
			TeleportTimer = 0.0f;
		}
	}
}

void ATeleporter2::StartTeleport()
{
	if (!bIsActive || bTeleportInProgress) return;

	bTeleportInProgress = true;
	TeleportTimer = TeleportDelay;

	// Activate visual effect
	if (TeleportEffect)
	{
		TeleportEffect->Activate();
	}
}

void ATeleporter2::ExecuteTeleport()
{
	if (!bIsActive) return;

	// Execute the level transition
	UGameplayStatics::OpenLevel(this, FName(*DestinationMapName));

	// Reset states
	bTeleportInProgress = false;
	TeleportTimer = 0.0f;
}

void ATeleporter2::ActivateTeleporter()
{
	if (!bIsActive) return;

	if (bPlayerInRange && !bTeleportInProgress)
	{
		StartTeleport();
	}
}

