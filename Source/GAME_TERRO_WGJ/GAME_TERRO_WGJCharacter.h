// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimSequence.h"
#include "Animation/BlendSpace.h"
#include "Animation/AnimInstance.h"
#include "Engine/Engine.h"
#include "GAME_TERRO_WGJCharacter.generated.h"

// Forward declarations
class USpringArmComponent;
class UCameraComponent;
class UAnimSequence;
class UBlendSpace;
class USkeletalMesh;
class UGameTerror;
class AHUD_terror;

UENUM()
enum class EBreathState : uint8
{
	Normal,
	Tired,
	Exhausted
};

UCLASS(config=Game)
class AGAME_TERRO_WGJCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	/** Camera boom positioning the character behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	// Animation transition variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Settings", meta = (AllowPrivateAccess = "true"))
	float RunSpeedThreshold;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Settings", meta = (AllowPrivateAccess = "true"))
	float WalkSpeedThreshold;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Settings", meta = (AllowPrivateAccess = "true"))
	float AnimationBlendTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Settings", meta = (AllowPrivateAccess = "true"))
	float SmoothTransitionSpeed;
	
	// Movement speeds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Settings", meta = (AllowPrivateAccess = "true"))
	float WalkSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Settings", meta = (AllowPrivateAccess = "true"))
	float RunSpeed;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement Settings", meta = (AllowPrivateAccess = "true"))
	float DefaultMaxWalkSpeed;

public:
	AGAME_TERRO_WGJCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	// Camera Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	bool bFirstPersonMode;

	// Animation Assets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Assets")
	USkeletalMesh* CyberpunkGirlMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Assets")
	UAnimSequence* IdleAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Assets")
	UAnimSequence* RunAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Assets")
	UAnimSequence* WalkAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Assets")
	UAnimSequence* JumpStartAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Assets")
	UBlendSpace* IdleRunBlendSpace;

	// Animation State Variables (Read-only for Blueprints)
	UPROPERTY(BlueprintReadOnly, Category = "Animation State")
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State")
	float Direction;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State")
	bool bIsMoving;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State")
	bool bIsRunning;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State")
	bool bIsJumping;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State")
	bool bIsFalling;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State")
	bool bWantsToRun;

	// Current animation state
	UPROPERTY(BlueprintReadOnly, Category = "Animation State")
	UAnimSequence* CurrentAnimation;

	/** Returns CameraBoom subobject **/
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	
	/** Returns FollowCamera subobject **/
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
	//////////////////////////////////////////////////////////////////////////
	// Core Overrides
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//////////////////////////////////////////////////////////////////////////
	// Input Functions
	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	//////////////////////////////////////////////////////////////////////////
	// Enhanced Movement Functions
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void StartRunning();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void StopRunning();

	//////////////////////////////////////////////////////////////////////////
	// Input Setup Functions
	void SetupInputMappings();
	void CreateRunInputMapping();
	void CreateJumpInputMapping();
	void CreateMovementInputMappings();

	//////////////////////////////////////////////////////////////////////////
	// Animation Functions
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void UpdateAnimationProperties();

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void SetupCyberpunkGirlMesh();

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void PlayAnimationBasedOnMovement();

	//////////////////////////////////////////////////////////////////////////
	// Camera Functions
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void ToggleFirstPerson();

	UFUNCTION(BlueprintCallable, Category = "Camera")
	void SetFirstPersonMode(bool bEnable);

	//////////////////////////////////////////////////////////////////////////
	// Jump Animation Functions
	UFUNCTION(BlueprintCallable, Category = "Jump Animation")
	void PlayJumpStartAnimation();

	// Override Jump function
	virtual void Jump() override;

	public:

	// ---- HUD ----
	UPROPERTY() AHUD_terror* HudActorRef = nullptr;

	// ---- Agotamiento ----
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Agotamiento", meta = (ClampMin = "0"))
	float agotamientoActual = 0.f;     

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Agotamiento", meta = (ClampMin = "0.01"))
	float tasaAumentoCorriendo = 0.20f;   

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Agotamiento", meta = (ClampMin = "0.01"))
	float tasaBajadaReposo = 0.30f;    

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Agotamiento")
	float delayRecuperacion = 0.75f;        

	UPROPERTY(BlueprintReadOnly, Category = "Agotamiento")
	bool bExhausto = false;

	float tiempoDesdeQueDejoDeCorrer = 0.f;

	void ActualizarAgotamiento(float DeltaTime);
	void ActualizarHUD();

	public:
		// ---- Audio: assets (as� puedes asignarlos en el editor o cargar por c�digo) ----
		UPROPERTY(EditAnywhere, Category = "Audio|Protagonista")
		USoundBase* SndSuspiroNormal = nullptr;          // Suspiros_chica_normal

		UPROPERTY(EditAnywhere, Category = "Audio|Protagonista")
		USoundBase* SndCansancio = nullptr;              // Cansancio_chica_

		UPROPERTY(EditAnywhere, Category = "Audio|Protagonista")
		USoundBase* SndSuperCansada = nullptr;           // Chica_s�per_cansada_

		UPROPERTY(EditAnywhere, Category = "Audio|Protagonista")
		USoundBase* SndGritoCorto = nullptr;             // Grito_corto_mujer_

		// (opcional) atenuaci�n si quieres radio 3D
		UPROPERTY(EditAnywhere, Category = "Audio|Protagonista")
		class USoundAttenuation* BreathAttenuation = nullptr;

protected:
	// Componente que dispara las voces/respiraci�n
	UPROPERTY(VisibleAnywhere, Category = "Audio|Protagonista")
	UAudioComponent* BreathComp = nullptr;

	// Estado y timings de respiraci�n
	EBreathState BreathState = EBreathState::Normal;
	float NextBreathTime = 0.f;

	// Umbrales de cansancio (0�1)
	UPROPERTY(EditAnywhere, Category = "Audio|Protagonista")
	float TiredThreshold = 0.60f;

	UPROPERTY(EditAnywhere, Category = "Audio|Protagonista")
	float ExhaustedThreshold = 0.90f;

	// Cooldown para no spamear el grito al entrar exhausto
	float ExhaustionGritoCooldown = 6.f;
	float NextAllowedGritoTime = 0.f;

	void UpdateBreathing(float Dt);
	void PlayBreathOnce(USoundBase* Snd, float Volume = 1.f);


};

