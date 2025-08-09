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
};

