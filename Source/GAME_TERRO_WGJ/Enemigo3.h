// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemigos.h"
#include "Animation/AnimSequence.h"
#include "Enemigo3.generated.h"

UCLASS()
class GAME_TERRO_WGJ_API AEnemigo3 : public AEnemigos
{
	GENERATED_BODY()
	
public:
	// Constructor
	AEnemigo3();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Duende animations
	UPROPERTY(BlueprintReadOnly, Category = "Duende Animations")
	UAnimSequence* IdleAnimation;

	UPROPERTY(BlueprintReadOnly, Category = "Duende Animations")
	UAnimSequence* RunAnimation;

	UPROPERTY(BlueprintReadOnly, Category = "Duende Animations")
	UAnimSequence* AttackAnimation;

	// Advanced AI state
	UPROPERTY(BlueprintReadOnly, Category = "Duende State")
	bool bIsChasing;

	UPROPERTY(BlueprintReadOnly, Category = "Duende State")
	bool bPlayerInSight;

	UPROPERTY(BlueprintReadOnly, Category = "Duende State")
	bool bIsAttacking;

	// Detection ranges with hysteresis
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Duende Stats")
	float DetectionRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Duende Stats")
	float LoseTargetRange;

	// Rotation settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Duende Stats")
	float RotationSpeed;

	// Movement settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Duende Stats")
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Duende Stats")
	float RunSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Duende Stats")
	float StoppingDistance;

	// Attack settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Duende Combat")
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Duende Combat")
	float AttackDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Duende Combat")
	float AttackCooldown;

	// Smoothing timer to avoid constant animation switching
	UPROPERTY(BlueprintReadOnly, Category = "Duende State")
	float StateChangeTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Duende Stats")
	float StateChangeDelay;

	// Attack timers
	UPROPERTY(BlueprintReadOnly, Category = "Duende State")
	float AttackTimer;

	UPROPERTY(BlueprintReadOnly, Category = "Duende State")
	float LastAttackTime;

private:
	// Animation functions
	void PlayIdleAnimation();
	void PlayRunAnimation();
	void PlayAttackAnimation();
	
	// Advanced behavior functions
	void UpdateBehavior();
	void UpdateDetection(float DeltaTime);
	void UpdateAttack(float DeltaTime);
	void RotateTowardsPlayer(float DeltaTime);
	void MoveTowardsPlayer(float DeltaTime);
	bool CanSeePlayer() const;
	bool CanAttackPlayer() const;
	void SmoothAnimationTransition(float DeltaTime);
};
