// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "Main.generated.h"

UENUM(BlueprintType)
enum class EMovementStatus : uint8
{
	EMS_Normal UMETA(DisplayName = "Normal"),
	EMS_Sprinting UMETA(DisplayName = "Sprinting"),

	EMS_MAX UMETA(Dis[layName = "DefaultMax"])
};

UENUM(BlueprintType)
enum class EStaminaStatus :uint8
{
	ESS_Normal UMETA(DisplayName = "Normal"),
	ESS_BelowMinimum UMETA(DisplayName = "BelowMinimum"),
	ESS_Exhausted UMETA(DisplayName = "Exhausted"),
	ESS_ExhaustedRecovering UMETA(DisplayName = "ExhaustedRecovering"),

	ESS_Max UMETA(DisplayName = "DefaultMax")
};

UCLASS()
class KNIGHTSKINGDOM_API AMain : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMain();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class UParticleSystem* HitParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class USoundCue* HitSound;

	TArray<FVector> PickupLocations;

	UFUNCTION(BlueprintCallable)
	void ShowPickupLocations();

	UPROPERTY(VisibleAnywhere , BlueprintReadWrite , Category ="Enums")
	EMovementStatus MovementStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EStaminaStatus StaminaStatus;

	FORCEINLINE void SetStaminaStatus(EStaminaStatus Status) { StaminaStatus = Status; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enums")
	float StaminaDrainRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enums")
	float MinSprintStamina;

	float InterpSpeed;
	bool bInterpToEnemy;
	void SetInterpToEnemy(bool Interp);

	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category ="Combat")
	class AEnemy* CombatTarget;

	FORCEINLINE void SetCombatTarget(AEnemy* Target) { CombatTarget = Target; }

	FRotator GetLookAtRotationYaw(FVector Target);

	/** Set movement status and running speed */
	void SetMovementStatus(EMovementStatus Status);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Running")
	float RunningSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Running")
	float SprintingSpeed;

	bool bShiftKeyDown;

	/** Pressed down to enable sprinting */
	void ShiftKeyDown();

	/** Released to Stop Sprinting */
	void ShiftKeyUp();

	/** Camera boom positioning the camera behind the player */
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Camera , meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow Camera */
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Camera , meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Base turn rate to scale turning functions for the camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	/**
	/*
	/*Player Stats
	/*
	*/

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly, Category="Player Stats")
	float MaxHealth;

	UPROPERTY(EditAnywhere , BlueprintReadWrite, Category = "Player Stats")
	float Health;

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly,  Category = "Player Stats")
	float MaxStamina;

	UPROPERTY(EditAnywhere , BlueprintReadWrite,  Category = "Player Stats")
	float Stamina;

	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = "Player Stats")
	int32 Coins;

	void DecrementHealth(float Amount);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, 
		class AController* EventInstigator, AActor* DamageCauser) override;

	void IncrementCoins(int32 Amount);	

	void Die();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Called for forwards/backwards input */
	void MoveForward(float input);
	
	/** Called for side to side input */
	void MoveRight(float input);

	/** Called via input to turn at a given rate 
	* @param Rate This is a normalized rate, i.e. 1 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);

	/** Called via input to look up/down at a given rate
	* @param Rate This is a normalized rate, i.e. 1 means 100% of desired look up/down rate
	*/
	void LookUpRate(float Rate);

	bool bLMBDown;
	void LMBDown();
	void LMBUp();

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Items")
	class AWeapon* EquippedWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Items")
	class AItem* ActiveOverlappingItem;

	void SetEquippedWeapon(AWeapon* WeaponToSet);
	FORCEINLINE AWeapon* GetEquippedWeapon(){ return EquippedWeapon ; }
	FORCEINLINE void SetActiveOverlappingItem(AItem* Item) { ActiveOverlappingItem = Item; }

	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category="Anims")
	bool bAttacking;

	void Attack();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	class UAnimMontage* CombatMontage;

	UFUNCTION(BlueprintCallable)
	void PlaySwingSound();
};
