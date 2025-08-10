// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemigo3.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/AudioComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemigo3::AEnemigo3()
{
	// Set this character to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;

	// Initialize advanced state
	bIsChasing = false;
	bPlayerInSight = false;
	bIsAttacking = false;
	DetectionRange = 800.0f;
	LoseTargetRange = 1200.0f; // Larger range to avoid flickering
	RotationSpeed = 180.0f; // Degrees per second
	StateChangeTimer = 0.0f;
	StateChangeDelay = 0.3f; // Wait 0.3 seconds before changing states
	
	// Initialize movement settings
	WalkSpeed = 200.0f; // Units per second when idle/patrolling
	RunSpeed = 400.0f;  // Units per second when chasing
	StoppingDistance = 150.0f; // Stop this distance from player
	
	// Initialize attack settings
	AttackRange = 120.0f; // Attack when this close to player
	AttackDuration = 1.5f; // How long attack animation plays
	AttackCooldown = 2.0f; // Wait time between attacks
	AttackTimer = 0.0f;
	LastAttackTime = 0.0f;
	
	// Configure Character Movement Component
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		GetCharacterMovement()->bOrientRotationToMovement = false; // We handle rotation manually
		GetCharacterMovement()->RotationRate = FRotator(0.0f, RotationSpeed, 0.0f);
	}

	// Load Duende Skeletal Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(
		TEXT("SkeletalMesh'/Game/duendeEscena3/Mesh/Skin_Body_A/SK_SkinBody_A.SK_SkinBody_A'")
	);
	if (MeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
		
		// Configure for direct animation control
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
		
		// Adjust mesh position and rotation for Duende
		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -88.0f));
		GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	}

	// Load Duende Animations
	// Load Idle Animation
	static ConstructorHelpers::FObjectFinder<UAnimSequence> IdleAnimAsset(
		TEXT("AnimSequence'/Game/duendeEscena3/Animations/Idle_A_01.Idle_A_01'")
	);
	if (IdleAnimAsset.Succeeded())
	{
		IdleAnimation = IdleAnimAsset.Object;
	}

	// Load Run Animation
	static ConstructorHelpers::FObjectFinder<UAnimSequence> RunAnimAsset(
		TEXT("AnimSequence'/Game/duendeEscena3/Animations/Run_Fwd_01.Run_Fwd_01'")
	);
	if (RunAnimAsset.Succeeded())
	{
		RunAnimation = RunAnimAsset.Object;
	}

	// Load Attack Animation
	static ConstructorHelpers::FObjectFinder<UAnimSequence> AttackAnimAsset(
		TEXT("AnimSequence'/Game/duendeEscena3/Animations/Attack_B_01.Attack_B_01'")
	);
	if (AttackAnimAsset.Succeeded())
	{
		AttackAnimation = AttackAnimAsset.Object;
	}

	// Create and configure Audio Component
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);
	AudioComponent->bAutoActivate = false; // Don't play automatically

	// Load Chase Sound
	static ConstructorHelpers::FObjectFinder<USoundWave> ChaseSoundAsset(
		TEXT("SoundWave'/Game/audioMonster/_Bonus/Scream/wav/Zombie_as_scream3_one_shot_03.Zombie_as_scream3_one_shot_03'")
	);
	if (ChaseSoundAsset.Succeeded())
	{
		ChaseSound = ChaseSoundAsset.Object;
	}

	// Initialize audio settings
	ChaseSoundInterval = 3.0f; // Play sound every 3 seconds while chasing
	ChaseSoundTimer = 0.0f;
}

void AEnemigo3::BeginPlay()
{
	Super::BeginPlay();
	
	// Start with idle animation
	PlayIdleAnimation();
}

void AEnemigo3::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Update advanced behavior system
	UpdateDetection(DeltaTime);
	UpdateAttack(DeltaTime);
	SmoothAnimationTransition(DeltaTime);
	UpdateChaseAudio(DeltaTime);
	
	// Move and rotate towards player when chasing (but not when attacking)
	if (bIsChasing && !bIsAttacking)
	{
		RotateTowardsPlayer(DeltaTime);
		MoveTowardsPlayer(DeltaTime);
	}
	// Still rotate when attacking to face player
	else if (bIsAttacking)
	{
		RotateTowardsPlayer(DeltaTime);
	}
}

void AEnemigo3::PlayIdleAnimation()
{
	if (IdleAnimation && GetMesh())
	{
		GetMesh()->PlayAnimation(IdleAnimation, true); // Loop = true
	}
}

void AEnemigo3::PlayRunAnimation()
{
	if (RunAnimation && GetMesh())
	{
		GetMesh()->PlayAnimation(RunAnimation, true); // Loop = true
	}
}

void AEnemigo3::PlayAttackAnimation()
{
	if (AttackAnimation && GetMesh())
	{
		GetMesh()->PlayAnimation(AttackAnimation, false); // Loop = false for attack
	}
}

void AEnemigo3::UpdateDetection(float DeltaTime)
{
	// Get player reference
	APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!Player) 
	{
		bPlayerInSight = false;
		return;
	}

	// Calculate distance to player
	float DistanceToPlayer = FVector::Dist(GetActorLocation(), Player->GetActorLocation());
	
	// Update detection state with hysteresis
	if (!bPlayerInSight && DistanceToPlayer <= DetectionRange)
	{
		// Player enters detection range
		bPlayerInSight = true;
		StateChangeTimer = 0.0f;
	}
	else if (bPlayerInSight && DistanceToPlayer > LoseTargetRange)
	{
		// Player leaves lose target range
		bPlayerInSight = false;
		StateChangeTimer = 0.0f;
	}
}

void AEnemigo3::SmoothAnimationTransition(float DeltaTime)
{
	// Don't change animations during attack
	if (bIsAttacking) return;
	
	// Update timer
	StateChangeTimer += DeltaTime;
	
	// Check for state transitions with delay
	if (StateChangeTimer >= StateChangeDelay)
	{
		// Priority 1: Attack if in range
		if (bPlayerInSight && CanAttackPlayer())
		{
			// Attack state is handled in UpdateAttack function
			return;
		}
		// Priority 2: Transition to chasing state
		else if (bPlayerInSight && !bIsChasing)
		{
			bIsChasing = true;
			PlayRunAnimation();
			StateChangeTimer = 0.0f;
		}
		// Priority 3: Transition to idle state
		else if (!bPlayerInSight && bIsChasing)
		{
			bIsChasing = false;
			PlayIdleAnimation();
			StateChangeTimer = 0.0f;
		}
	}
}

void AEnemigo3::RotateTowardsPlayer(float DeltaTime)
{
	APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!Player) return;

	// Calculate direction to player
	FVector DirectionToPlayer = (Player->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	
	// Calculate target rotation
	FRotator TargetRotation = DirectionToPlayer.Rotation();
	
	// Get current rotation
	FRotator CurrentRotation = GetActorRotation();
	
	// Smoothly rotate towards target
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed / 180.0f);
	
	// Only rotate on Z-axis (Yaw)
	NewRotation.Pitch = CurrentRotation.Pitch;
	NewRotation.Roll = CurrentRotation.Roll;
	
	SetActorRotation(NewRotation);
}

void AEnemigo3::UpdateAttack(float DeltaTime)
{
	// Update attack timer
	AttackTimer += DeltaTime;
	
	// Check if we can attack the player
	if (CanAttackPlayer() && !bIsAttacking)
	{
		// Check cooldown
		float TimeSinceLastAttack = GetWorld()->GetTimeSeconds() - LastAttackTime;
		if (TimeSinceLastAttack >= AttackCooldown)
		{
			// Start attack
			bIsAttacking = true;
			AttackTimer = 0.0f;
			LastAttackTime = GetWorld()->GetTimeSeconds();
			PlayAttackAnimation();
			
			// Stop movement during attack
			if (GetCharacterMovement())
			{
				GetCharacterMovement()->StopMovementImmediately();
			}
		}
	}
	
	// End attack after duration
	if (bIsAttacking && AttackTimer >= AttackDuration)
	{
		bIsAttacking = false;
		AttackTimer = 0.0f;
		
		// Return to appropriate state
		if (bPlayerInSight)
		{
			if (CanAttackPlayer())
			{
				PlayIdleAnimation(); // Stay close but don't move
			}
			else
			{
				PlayRunAnimation(); // Resume chasing
			}
		}
		else
		{
			PlayIdleAnimation(); // Lost player
		}
	}
}

bool AEnemigo3::CanAttackPlayer() const
{
	APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!Player) return false;

	float DistanceToPlayer = FVector::Dist(GetActorLocation(), Player->GetActorLocation());
	return DistanceToPlayer <= AttackRange;
}

void AEnemigo3::MoveTowardsPlayer(float DeltaTime)
{
	APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!Player) return;

	// Calculate distance to player
	float DistanceToPlayer = FVector::Dist(GetActorLocation(), Player->GetActorLocation());
	
	// Don't move if within attack range
	if (DistanceToPlayer <= AttackRange)
	{
		// Stop movement - ready to attack
		if (GetCharacterMovement())
		{
			GetCharacterMovement()->StopMovementImmediately();
		}
		return;
	}
	
	// Don't move if too close to player (stopping distance)
	if (DistanceToPlayer <= StoppingDistance)
	{
		// Stop movement
		if (GetCharacterMovement())
		{
			GetCharacterMovement()->StopMovementImmediately();
		}
		return;
	}

	// Calculate direction to player
	FVector DirectionToPlayer = (Player->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	
	// Update movement speed based on state
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = bIsChasing ? RunSpeed : WalkSpeed;
	}
	
	// Add movement input in the direction of the player
	AddMovementInput(DirectionToPlayer, 1.0f);
}

bool AEnemigo3::CanSeePlayer() const
{
	APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!Player) return false;

	// Simple line of sight check (can be enhanced with raycasting)
	float DistanceToPlayer = FVector::Dist(GetActorLocation(), Player->GetActorLocation());
	return DistanceToPlayer <= DetectionRange;
}

void AEnemigo3::UpdateBehavior()
{
	// Legacy function - now handled by UpdateDetection and SmoothAnimationTransition
	// Keeping for compatibility
}

void AEnemigo3::UpdateChaseAudio(float DeltaTime)
{
	// Only play chase sounds when actively chasing and not attacking
	if (bIsChasing && !bIsAttacking)
	{
		ChaseSoundTimer += DeltaTime;
		
		// Play chase sound at intervals
		if (ChaseSoundTimer >= ChaseSoundInterval)
		{
			PlayChaseSound();
			ChaseSoundTimer = 0.0f;
		}
	}
	else
	{
		// Reset timer when not chasing
		ChaseSoundTimer = 0.0f;
	}
}

void AEnemigo3::PlayChaseSound()
{
	if (ChaseSound && AudioComponent)
	{
		// Set the sound and play it
		AudioComponent->SetSound(ChaseSound);
		AudioComponent->Play();
		
		// Optional: Add debug message
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Duende risa!"));
		}
	}
}

