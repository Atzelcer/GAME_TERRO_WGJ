// Copyright Epic Games, Inc. All Rights Reserved.

#include "GAME_TERRO_WGJCharacter.h"

// Unreal Engine includes
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

// Animation includes
#include "Animation/AnimSequence.h"
#include "Animation/BlendSpace.h"
#include "Animation/AnimInstance.h"

// Utility includes
#include "Engine/Engine.h"
#include "Kismet/KismetMathLibrary.h"
#include "UObject/ConstructorHelpers.h"

//////////////////////////////////////////////////////////////////////////
// AGAME_TERRO_WGJCharacter Constructor

AGAME_TERRO_WGJCharacter::AGAME_TERRO_WGJCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 400.f; // Reduced jump height for more realistic jumping
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Initialize animation variables with safe defaults
	Speed = 0.0f;
	Direction = 0.0f;
	bIsMoving = false;
	bIsRunning = false;
	bIsJumping = false;
	bIsFalling = false;
	bWantsToRun = false;
	
	// Enhanced thresholds for smooth transitions
	RunSpeedThreshold = 350.0f;
	WalkSpeedThreshold = 5.0f;
	AnimationBlendTime = 0.15f;
	SmoothTransitionSpeed = 8.0f;
	
	// Movement speeds configuration
	WalkSpeed = 200.0f;
	RunSpeed = 600.0f;
	DefaultMaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	
	// Initialize animation pointers to null for safety
	CyberpunkGirlMesh = nullptr;
	IdleAnimation = nullptr;
	RunAnimation = nullptr;
	WalkAnimation = nullptr;
	JumpStartAnimation = nullptr;
	IdleRunBlendSpace = nullptr;
	CurrentAnimation = nullptr;

	// Enable tick for animation updates
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Setup input mappings programmatically
	SetupInputMappings();

	// Setup Cyberpunk Girl Mesh and Animations with asset references
	SetupCyberpunkGirlMesh();
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGAME_TERRO_WGJCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	//////////////////////////////////////////////////////////////////////////
	// Jump Actions - Space Bar
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//////////////////////////////////////////////////////////////////////////
	// Run Actions - Shift keys (mapped in DefaultInput.ini)
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AGAME_TERRO_WGJCharacter::StartRunning);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AGAME_TERRO_WGJCharacter::StopRunning);

	//////////////////////////////////////////////////////////////////////////
	// Movement Axis - WASD
	PlayerInputComponent->BindAxis("MoveForward", this, &AGAME_TERRO_WGJCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGAME_TERRO_WGJCharacter::MoveRight);

	//////////////////////////////////////////////////////////////////////////
	// Camera Controls - Mouse
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AGAME_TERRO_WGJCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AGAME_TERRO_WGJCharacter::LookUpAtRate);

	//////////////////////////////////////////////////////////////////////////
	// Touch Controls
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AGAME_TERRO_WGJCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AGAME_TERRO_WGJCharacter::TouchStopped);

	//////////////////////////////////////////////////////////////////////////
	// VR Controls
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AGAME_TERRO_WGJCharacter::OnResetVR);

	// Debug: Log successful setup
	UE_LOG(LogTemp, Log, TEXT("Player Input Component Setup Complete - Run action mapped to Shift keys"));
}


void AGAME_TERRO_WGJCharacter::OnResetVR()
{
	// If GAME_TERRO_WGJ is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in GAME_TERRO_WGJ.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AGAME_TERRO_WGJCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AGAME_TERRO_WGJCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AGAME_TERRO_WGJCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AGAME_TERRO_WGJCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AGAME_TERRO_WGJCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(ForwardDirection, Value);
	}
}

void AGAME_TERRO_WGJCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(RightDirection, Value);
	}
}

//////////////////////////////////////////////////////////////////////////
// Animation System Implementation

void AGAME_TERRO_WGJCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Load animation assets from content folder
	SetupCyberpunkGirlMesh();
}

void AGAME_TERRO_WGJCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Update animation properties every frame
	UpdateAnimationProperties();
	
	// Handle animation transitions
	PlayAnimationBasedOnMovement();
	
	// Debug: Check input status periodically (every second)
	#if WITH_EDITOR
	static float DebugTimer = 0.0f;
	DebugTimer += DeltaTime;
	if (DebugTimer >= 1.0f)
	{
		CheckInputStatus();
		DebugTimer = 0.0f;
	}
	#endif
}

void AGAME_TERRO_WGJCharacter::SetupCyberpunkGirlMesh()
{
	// Load Cyberpunk Girl Skeletal Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/Cyberpunk_Girl/Mesh/SK_Cyberpunk_Girl"));
	if (MeshAsset.Succeeded())
	{
		CyberpunkGirlMesh = MeshAsset.Object;
		GetMesh()->SetSkeletalMesh(CyberpunkGirlMesh);
		
		// Adjust mesh position and rotation
		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -88.0f));
		GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
		
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Cyberpunk Girl Mesh Loaded Successfully!"));
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Failed to load Cyberpunk Girl Mesh!"));
		}
	}

	// Load Animation Assets
	static ConstructorHelpers::FObjectFinder<UAnimSequence> IdleAnimAsset(TEXT("/Game/Cyberpunk_Girl/Demo/ThirdPersonIdle"));
	if (IdleAnimAsset.Succeeded())
	{
		IdleAnimation = IdleAnimAsset.Object;
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Idle Animation Loaded!"));
		}
	}

	static ConstructorHelpers::FObjectFinder<UAnimSequence> RunAnimAsset(TEXT("/Game/Cyberpunk_Girl/Demo/ThirdPersonRun"));
	if (RunAnimAsset.Succeeded())
	{
		RunAnimation = RunAnimAsset.Object;
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Run Animation Loaded!"));
		}
	}

	static ConstructorHelpers::FObjectFinder<UAnimSequence> WalkAnimAsset(TEXT("/Game/Cyberpunk_Girl/Demo/ThirdPersonWalk"));
	if (WalkAnimAsset.Succeeded())
	{
		WalkAnimation = WalkAnimAsset.Object;
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Walk Animation Loaded!"));
		}
	}

	// Load Jump Start Animation
	static ConstructorHelpers::FObjectFinder<UAnimSequence> JumpStartAnimAsset(TEXT("/Game/Cyberpunk_Girl/Demo/ThirdPersonJump_Start"));
	if (JumpStartAnimAsset.Succeeded())
	{
		JumpStartAnimation = JumpStartAnimAsset.Object;
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Jump Start Animation Loaded!"));
		}
	}

	// Start with idle animation
	if (IdleAnimation && GetMesh())
	{
		CurrentAnimation = IdleAnimation;
		GetMesh()->PlayAnimation(IdleAnimation, true);
	}
}

void AGAME_TERRO_WGJCharacter::UpdateAnimationProperties()
{
	// Safety check
	if (!GetCharacterMovement())
	{
		return;
	}

	// Calculate current speed
	FVector Velocity = GetVelocity();
	Speed = Velocity.Size();
	
	// Update movement states
	bIsMoving = Speed > WalkSpeedThreshold;
	bIsJumping = GetCharacterMovement()->IsFalling();
	bIsFalling = GetCharacterMovement()->IsFalling() && Velocity.Z < -100.0f;
	
	// Update running state based on input and movement
	if (bWantsToRun && bIsMoving)
	{
		bIsRunning = Speed > WalkSpeedThreshold;
		// Smooth transition to run speed
		float TargetSpeed = RunSpeed;
		float CurrentMaxSpeed = GetCharacterMovement()->MaxWalkSpeed;
		GetCharacterMovement()->MaxWalkSpeed = FMath::FInterpTo(
			CurrentMaxSpeed, 
			TargetSpeed, 
			GetWorld()->GetDeltaSeconds(), 
			SmoothTransitionSpeed
		);
	}
	else
	{
		bIsRunning = false;
		// Smooth transition to walk speed
		float TargetSpeed = WalkSpeed;
		float CurrentMaxSpeed = GetCharacterMovement()->MaxWalkSpeed;
		GetCharacterMovement()->MaxWalkSpeed = FMath::FInterpTo(
			CurrentMaxSpeed, 
			TargetSpeed, 
			GetWorld()->GetDeltaSeconds(), 
			SmoothTransitionSpeed
		);
	}

	// Debug output (only in development builds)
	#if WITH_EDITOR
	if (GEngine)
	{
		FString DebugMessage = FString::Printf(
			TEXT("Speed: %.1f | Moving: %s | Running: %s | Wants Run: %s | Jumping: %s"), 
			Speed, 
			bIsMoving ? TEXT("True") : TEXT("False"), 
			bIsRunning ? TEXT("True") : TEXT("False"),
			bWantsToRun ? TEXT("True") : TEXT("False"),
			bIsJumping ? TEXT("True") : TEXT("False")
		);
		GEngine->AddOnScreenDebugMessage(0, 0.0f, FColor::White, DebugMessage);
	}
	#endif
}

void AGAME_TERRO_WGJCharacter::PlayAnimationBasedOnMovement()
{
	// Safety checks
	if (!GetMesh())
	{
		UE_LOG(LogTemp, Warning, TEXT("GetMesh() returned null in PlayAnimationBasedOnMovement"));
		return;
	}

	UAnimSequence* TargetAnimation = nullptr;

	// Animation priority system
	// 1. Jump animation (highest priority)
	if (bIsJumping && JumpStartAnimation)
	{
		TargetAnimation = JumpStartAnimation;
	}
	// 2. Ground movement animations
	else if (!bIsMoving && IdleAnimation)
	{
		TargetAnimation = IdleAnimation;
	}
	else if (bIsRunning && bWantsToRun && RunAnimation)
	{
		TargetAnimation = RunAnimation;
	}
	else if (bIsMoving && WalkAnimation)
	{
		TargetAnimation = WalkAnimation;
	}
	// 3. Fallback animation
	else if (IdleAnimation)
	{
		TargetAnimation = IdleAnimation;
	}

	// Only change animation if it's different from current and target is valid
	if (TargetAnimation && TargetAnimation != CurrentAnimation)
	{
		CurrentAnimation = TargetAnimation;
		
		// Determine if animation should loop
		bool bShouldLoop = (TargetAnimation == IdleAnimation || 
		                   TargetAnimation == RunAnimation || 
		                   TargetAnimation == WalkAnimation);
		
		// Play the animation directly on the skeletal mesh
		GetMesh()->PlayAnimation(TargetAnimation, bShouldLoop);
		
		// Debug output (only in development builds)
		#if WITH_EDITOR
		if (GEngine)
		{
			FString AnimName = TargetAnimation->GetName();
			FString DebugMessage = FString::Printf(
				TEXT("Playing Animation: %s (Speed: %.1f, Loop: %s)"), 
				*AnimName, 
				Speed,
				bShouldLoop ? TEXT("Yes") : TEXT("No")
			);
			GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Yellow, DebugMessage);
		}
		#endif
	}
}

//////////////////////////////////////////////////////////////////////////
// Enhanced Movement System

void AGAME_TERRO_WGJCharacter::StartRunning()
{
	bWantsToRun = true;
	
	#if WITH_EDITOR
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(2, 2.0f, FColor::Cyan, TEXT("🏃 Started Running! (Shift Pressed)"));
	}
	#endif
	
	UE_LOG(LogTemp, Log, TEXT("StartRunning called - bWantsToRun set to true"));
}

void AGAME_TERRO_WGJCharacter::StopRunning()
{
	bWantsToRun = false;
	
	#if WITH_EDITOR
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(2, 2.0f, FColor::Cyan, TEXT("🚶 Stopped Running! (Shift Released)"));
	}
	#endif
	
	UE_LOG(LogTemp, Log, TEXT("StopRunning called - bWantsToRun set to false"));
}

//////////////////////////////////////////////////////////////////////////
// Input Setup Functions

void AGAME_TERRO_WGJCharacter::SetupInputMappings()
{
	// Log that we're setting up input mappings
	UE_LOG(LogTemp, Log, TEXT("Setting up input mappings programmatically"));
	
	// Most input mappings will be handled directly in SetupPlayerInputComponent
	// This function can be used for any additional setup if needed
}

void AGAME_TERRO_WGJCharacter::CreateRunInputMapping()
{
	// This function is simplified - direct key binding is handled in SetupPlayerInputComponent
	UE_LOG(LogTemp, Log, TEXT("Run input mapping setup - using direct key binding"));
}

void AGAME_TERRO_WGJCharacter::CreateJumpInputMapping()
{
	// This function is simplified - direct key binding is handled in SetupPlayerInputComponent
	UE_LOG(LogTemp, Log, TEXT("Jump input mapping setup - using direct key binding"));
}

void AGAME_TERRO_WGJCharacter::CreateMovementInputMappings()
{
	// This function is simplified - axis mappings handled by default input settings
	UE_LOG(LogTemp, Log, TEXT("Movement input mappings setup - using default mappings"));
}

//////////////////////////////////////////////////////////////////////////
// Debug Functions

void AGAME_TERRO_WGJCharacter::CheckInputStatus()
{
	#if WITH_EDITOR
	if (GEngine)
	{
		FString InputStatus = FString::Printf(
			TEXT("🎮 INPUT STATUS:\nbWantsToRun: %s\nbIsRunning: %s\nbIsMoving: %s\nSpeed: %.1f"),
			bWantsToRun ? TEXT("TRUE") : TEXT("FALSE"),
			bIsRunning ? TEXT("TRUE") : TEXT("FALSE"),
			bIsMoving ? TEXT("TRUE") : TEXT("FALSE"),
			Speed
		);
		
		FString JumpStatus = FString::Printf(
			TEXT("🚀 JUMP STATUS:\nbIsJumping: %s\nbIsFalling: %s"),
			bIsJumping ? TEXT("TRUE") : TEXT("FALSE"),
			bIsFalling ? TEXT("TRUE") : TEXT("FALSE")
		);
		
		GEngine->AddOnScreenDebugMessage(10, 1.0f, FColor::Orange, InputStatus);
		GEngine->AddOnScreenDebugMessage(11, 1.0f, FColor::Cyan, JumpStatus);
	}
	#endif
}

//////////////////////////////////////////////////////////////////////////
// Jump Animation System (Simplified)

void AGAME_TERRO_WGJCharacter::Jump()
{
	// Call parent Jump function first
	Super::Jump();
	
	// Play the jump start animation
	PlayJumpStartAnimation();
}

void AGAME_TERRO_WGJCharacter::PlayJumpStartAnimation()
{
	if (JumpStartAnimation && GetMesh())
	{
		CurrentAnimation = JumpStartAnimation;
		GetMesh()->PlayAnimation(JumpStartAnimation, false); // Don't loop
		
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Cyan, TEXT("🚀 Playing Jump Start Animation!"));
		}
	}
}
