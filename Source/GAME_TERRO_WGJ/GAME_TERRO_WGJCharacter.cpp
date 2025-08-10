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

#include "GameTerror.h"
#include "HUD_terror.h"

#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"

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
	
	// Camera settings
	bFirstPersonMode = false; // Start in third person mode (more stable)
	
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

	BreathComp = CreateDefaultSubobject<UAudioComponent>(TEXT("BreathComp"));
	BreathComp->SetupAttachment(RootComponent);
	BreathComp->bAutoActivate = false;
	BreathComp->bIsUISound = false;

	//static ConstructorHelpers::FObjectFinder<USoundBase> S1(
	//	TEXT("SoundWave'/Game/MoreSounds/Sonidos/Personajes/Protagonista/Suspiros_chica_normal.Suspiros_chica_normal'"));
	//if (S1.Succeeded()) SndSuspiroNormal = S1.Object;

	//static ConstructorHelpers::FObjectFinder<USoundBase> S2(
	//	TEXT("SoundWave'/Game/MoreSounds/Sonidos/Personajes/Protagonista/Cansancio_chica_.Cansancio_chica_'"));
	//if (S2.Succeeded()) SndCansancio = S2.Object;

	//static ConstructorHelpers::FObjectFinder<USoundBase> S3(
	//	TEXT("SoundWave'/Game/MoreSounds/Sonidos/Personajes/Protagonista/Chica_súper_cansada_.Chica_súper_cansada_'"));
	//if (S3.Succeeded()) SndSuperCansada = S3.Object;

	//static ConstructorHelpers::FObjectFinder<USoundBase> S4(
	//	TEXT("SoundWave'/Game/MoreSounds/Sonidos/Personajes/Protagonista/Grito_corto_mujer_.Grito_corto_mujer_'"));
	//if (S4.Succeeded()) SndGritoCorto = S4.Object;

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

	// Note: Camera toggle can be called from Blueprints using ToggleFirstPerson()

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
	NextBreathTime = GetWorld()->GetTimeSeconds() + 1.0f;
	SetupCyberpunkGirlMesh();

	// cachea el HUD del jugador
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		HudActorRef = PC->GetHUD<AHUD_terror>();
	}
	else
	{
		// alternativa si BeginPlay corre antes de poseer:
		if (APlayerController* PC0 = GetWorld()->GetFirstPlayerController())
		{
			HudActorRef = PC0->GetHUD<AHUD_terror>();
		}
	}

	if (HudActorRef)
	{
		HudActorRef->EstablecerCansancio(agotamientoActual);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("HUD actor linked"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("HUD actor not found (check GameMode HUDClass)"));
	}

	SetFirstPersonMode(true);
}


void AGAME_TERRO_WGJCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Update animation properties every frame
	UpdateAnimationProperties();
	
	// Handle animation transitions
	PlayAnimationBasedOnMovement();

	ActualizarAgotamiento(DeltaTime);
	ActualizarHUD();

	UpdateBreathing(DeltaTime);
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
	}

	// Load Animation Assets
	static ConstructorHelpers::FObjectFinder<UAnimSequence> IdleAnimAsset(TEXT("/Game/Cyberpunk_Girl/Demo/ThirdPersonIdle"));
	if (IdleAnimAsset.Succeeded())
	{
		IdleAnimation = IdleAnimAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimSequence> RunAnimAsset(TEXT("/Game/Cyberpunk_Girl/Demo/ThirdPersonRun"));
	if (RunAnimAsset.Succeeded())
	{
		RunAnimation = RunAnimAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimSequence> WalkAnimAsset(TEXT("/Game/Cyberpunk_Girl/Demo/ThirdPersonWalk"));
	if (WalkAnimAsset.Succeeded())
	{
		WalkAnimation = WalkAnimAsset.Object;
	}

	// Load Jump Start Animation
	static ConstructorHelpers::FObjectFinder<UAnimSequence> JumpStartAnimAsset(TEXT("/Game/Cyberpunk_Girl/Demo/ThirdPersonJump_Start"));
	if (JumpStartAnimAsset.Succeeded())
	{
		JumpStartAnimation = JumpStartAnimAsset.Object;
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
	}
}

//////////////////////////////////////////////////////////////////////////
// Enhanced Movement System

void AGAME_TERRO_WGJCharacter::StartRunning()
{
	if (!bExhausto)
	{
		bWantsToRun = true;
	}
}

void AGAME_TERRO_WGJCharacter::StopRunning()
{
	bWantsToRun = false;
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
// Jump Animation System (Simplified)

void AGAME_TERRO_WGJCharacter::Jump()
{
	// Call parent Jump function first
	Super::Jump();
	
	// Play the jump start animation
	PlayJumpStartAnimation();
}

void AGAME_TERRO_WGJCharacter::ActualizarAgotamiento(float Dt)
{
	const bool estaMoviendose = bIsMoving;
	const bool manteniendoRun = bWantsToRun;

	// si está exhausto, fuerza caminar
	if (bExhausto)
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}

	// aumento de agotamiento
	if (estaMoviendose && manteniendoRun && !bExhausto)
	{
		agotamientoActual += tasaAumentoCorriendo * Dt;
		tiempoDesdeQueDejoDeCorrer = 0.f;
	}
	else
	{
		// recuperación
		tiempoDesdeQueDejoDeCorrer += Dt;
		if (tiempoDesdeQueDejoDeCorrer >= delayRecuperacion)
		{
			agotamientoActual -= tasaBajadaReposo * Dt;

		}
	}

	agotamientoActual = FMath::Clamp(agotamientoActual, 0.f, 1.f);

	// entrar en estado exhausto
	if (agotamientoActual >= 1.f)
	{
		if (!bExhausto)
		bExhausto = true;
	}

	// salir de estado exhausto
	if (bExhausto && agotamientoActual <= 0.25f)
	{
		bExhausto = false;
	}

	if (bExhausto)
	{
		bWantsToRun = false;
	}
}


void AGAME_TERRO_WGJCharacter::ActualizarHUD()
{
	if (HudActorRef)
	{
		HudActorRef->EstablecerCansancio(agotamientoActual);
	}
}

void AGAME_TERRO_WGJCharacter::UpdateBreathing(float /*Dt*/)
{
	// Determina estado
	EBreathState NewState = EBreathState::Normal;
	if (agotamientoActual >= ExhaustedThreshold)       NewState = EBreathState::Exhausted;
	else if (agotamientoActual >= TiredThreshold)      NewState = EBreathState::Tired;

	// Si entraste a Exhausted por primera vez (o tras recuperarte), permite un grito corto
	const float Now = GetWorld()->GetTimeSeconds();
	static bool bPrevExhausted = false;
	const bool bNowExhausted = (NewState == EBreathState::Exhausted);

	if (bNowExhausted && !bPrevExhausted && SndGritoCorto && Now >= NextAllowedGritoTime)
	{
		PlayBreathOnce(SndGritoCorto, 1.0f);
		NextAllowedGritoTime = Now + ExhaustionGritoCooldown; // cooldown
	}
	bPrevExhausted = bNowExhausted;

	// Actualiza el estado
	if (NewState != BreathState)
	{
		BreathState = NewState;
		// Fuerza un suspiro inmediato al cambiar de estado
		NextBreathTime = Now;
	}

	// ¿toca reproducir un suspiro?
	if (Now >= NextBreathTime)
	{
		USoundBase* ToPlay = nullptr;
		float volume = 1.f;
		float nextMin = 1.8f, nextMax = 3.0f;

		switch (BreathState)
		{
		case EBreathState::Normal:
			ToPlay = SndSuspiroNormal;
			volume = 0.9f;
			nextMin = 2.5f; nextMax = 4.0f;
			break;
		case EBreathState::Tired:
			ToPlay = SndCansancio;
			volume = 1.0f;
			nextMin = 1.2f; nextMax = 2.0f;
			break;
		case EBreathState::Exhausted:
			ToPlay = SndSuperCansada;
			volume = 1.0f;
			nextMin = 0.8f; nextMax = 1.5f;
			break;
		}

		if (ToPlay)
		{
			PlayBreathOnce(ToPlay, volume);
		}

		// Programa el próximo suspiro en un rango aleatorio (para que no sea robótico)
		const float interval = FMath::FRandRange(nextMin, nextMax);
		NextBreathTime = Now + interval;
	}
}

void AGAME_TERRO_WGJCharacter::PlayBreathOnce(USoundBase* Snd, float Volume)
{
	if (!Snd || !BreathComp) return;
	BreathComp->SetSound(Snd);
	BreathComp->SetVolumeMultiplier(Volume);
	BreathComp->Play(0.f);
}


void AGAME_TERRO_WGJCharacter::PlayJumpStartAnimation()
{
	if (JumpStartAnimation && GetMesh())
	{
		CurrentAnimation = JumpStartAnimation;
		GetMesh()->PlayAnimation(JumpStartAnimation, false); // Don't loop
	}
}

//////////////////////////////////////////////////////////////////////////
// Camera System (Simplified)

void AGAME_TERRO_WGJCharacter::ToggleFirstPerson()
{
	bFirstPersonMode = !bFirstPersonMode;
	SetFirstPersonMode(bFirstPersonMode);
}

void AGAME_TERRO_WGJCharacter::SetFirstPersonMode(bool bEnable)
{
	bFirstPersonMode = bEnable;

	if (bFirstPersonMode)
	{
		CameraBoom->TargetArmLength = 0.f;
		CameraBoom->bUsePawnControlRotation = false;
		CameraBoom->bDoCollisionTest = false;

		FollowCamera->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		FollowCamera->SetRelativeLocation(FVector(0.f, 0.f, 60.f));
		FollowCamera->SetRelativeRotation(FRotator::ZeroRotator);
		FollowCamera->bUsePawnControlRotation = true;

		bUseControllerRotationYaw = true;
		bUseControllerRotationPitch = true;
		bUseControllerRotationRoll = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;

		if (GetMesh()) GetMesh()->SetOwnerNoSee(true);
	}
	else
	{
		FollowCamera->AttachToComponent(CameraBoom, FAttachmentTransformRules::SnapToTargetNotIncludingScale, USpringArmComponent::SocketName);
		FollowCamera->SetRelativeLocation(FVector::ZeroVector);
		FollowCamera->SetRelativeRotation(FRotator::ZeroRotator);
		FollowCamera->bUsePawnControlRotation = false;

		CameraBoom->TargetArmLength = 300.f;
		CameraBoom->bUsePawnControlRotation = true;
		CameraBoom->bDoCollisionTest = true;

		bUseControllerRotationYaw = false;
		bUseControllerRotationPitch = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;

		if (GetMesh()) GetMesh()->SetOwnerNoSee(false);
	}
}
