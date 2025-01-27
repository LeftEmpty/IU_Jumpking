// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "JCharacter.generated.h"


class IJInteractInterface;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnRecoursesChangedDelegate, AJCharacter*, Player, int32, Lives, int32, Coins, bool, HasKey);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDeathDelegate, AJCharacter*, Player, bool, bGameOver);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVictoryDelegate, AJCharacter*, Player);


UENUM()
enum EPlayerState{
	Idle			UMETA(DisplayName = "Idle"),
	Moving			UMETA(DisplayName = "Moving"),
	MidAir			UMETA(DisplayName = "Mid Air"),
	JumpHolding		UMETA(DisplayName = "JumpHolding"),
	Won				UMETA(DisplayName = "Won"),
	GameOver		UMETA(DisplayName = "Game Over")
};


UCLASS()
class IU_JUMPKING_API AJCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AJCharacter();

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Capsule component that looks for interactables */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleColliderComp;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	// Use these delegates to update the UI
	UPROPERTY(BlueprintAssignable, Category = "Sunny")
	FOnRecoursesChangedDelegate OnRecoursesChanged;

	UPROPERTY(BlueprintAssignable, Category = "Sunny")
	FOnDeathDelegate OnDeath;

	UPROPERTY(BlueprintAssignable, Category = "Sunny")
	FOnVictoryDelegate OnVictory;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// Player State
	UFUNCTION(BlueprintCallable, Category = "Sunny|Jump")
	void SetPlayerState(EPlayerState NewState);

	UFUNCTION(BlueprintCallable, Category = "Sunny|Jump")
	EPlayerState GetPlayerState() { return PS; }

	// Resources
	UFUNCTION(BlueprintCallable, Category = "Sunny|Recourses")
	void AddCoins(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Sunny|Recourses")
	int32 GetCoins() { return Coins; }

	UFUNCTION(BlueprintCallable, Category = "Sunny|Recourses")
	void AddLives(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Sunny|Recourses")
	int32 GetLives() { return Lives; }

	UFUNCTION(BlueprintCallable, Category = "Sunny|Recourses")
	void SetHasKey(bool HasKey);

	UFUNCTION(BlueprintCallable, Category = "Sunny|Recourses")
	bool GetHasKey() { return bHasKey; }
	
	// Death
	UFUNCTION(BlueprintCallable, Category ="Sunny")
	void Die();

	UFUNCTION(BlueprintCallable, Category ="Sunny")
	void GameOver();
	
	UFUNCTION(BlueprintCallable, Category ="Sunny")
	void Respawn();

	// Checkpoints
	void UpdateCheckpoint(FVector CheckpointSpawnLocation);

	UFUNCTION(BlueprintCallable, Category = "Sunny")
	FVector GetLastCheckpointLocation() { return LastCheckpointLoc; }
	
	// Goal / Victory
	UFUNCTION()
	void Win();

	// Level Switching
	void ChangeToLevel(FString Level);

	TArray<FString> Levels;

protected:
	// Interaction
	UFUNCTION()
	void OnInteractableNoticed(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnInteractableForgotten(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	TArray<IJInteractInterface*> InteractablesInRange;
	
	UFUNCTION()
	void Interact();

	// Jumping
	UPROPERTY(EditDefaultsOnly, Category = "Sunny|Jump")
	float MaxJumpHeightVelocity; // max height the player should be able to jump

	UPROPERTY(EditDefaultsOnly, Category = "Sunny|Jump")
	float BaseJumpHeightVelocity = 200.f; // multiplier to adjust for increased gravity

	UPROPERTY(EditDefaultsOnly, Category = "Sunny|Jump")
	float JumpHeightVelocity; // velocity set in code depending on how long key is held
	
	UPROPERTY(EditDefaultsOnly, Category = "Sunny|Jump")
	float BaseJumpForwardVelocity = 200.f; // multiplier to adjust for increased gravity

	UPROPERTY(EditDefaultsOnly, Category = "Sunny|Jump")
	float JumpForwardVelocity = 500; // velocity set in code depending on how long key is held

	float JumpKeyDownTime;

	bool bJumpKeyDown;

	float MoveSpeed; // temporary stores move speed variable for stopping movement during jump

	// Death
	FTimerHandle FOnDeathTimer;

	UPROPERTY(EditDefaultsOnly, Category="Sunny")
	FVector PlayerStartVec;

	// FX
	UPROPERTY(EditDefaultsOnly, Category="Sunny|FX")
	class USoundBase* DeathSound;

	UPROPERTY(EditDefaultsOnly, Category="Sunny|FX")
	class UNiagaraSystem* DeathEffect;

	// Victory
	UPROPERTY(EditDefaultsOnly, Category = "Sunny|Jump")
	class UAnimMontage* VictoryAnimation;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for jump input - pressed */
	void JumpStart();

	/** Called for jump input - released */
	void JumpEnd();

	void DeathFX();

private:
	EPlayerState PS;

	int32 Coins;

	int32 Lives;

	int32 MaxLives;

	bool bHasKey;

	bool bCheckpointReached;

	bool bDead;

	FVector LastCheckpointLoc;
};

