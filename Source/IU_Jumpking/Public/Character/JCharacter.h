// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "JCharacter.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnRecoursesChangedDelegate, AJCharacter*, Player, int32, Lives, int32, Coins, bool, HasKey);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameOverDelegate, AJCharacter*, Player);


UENUM()
enum EPlayerState{
	Idle			UMETA(DisplayName = "Idle"),
	Moving			UMETA(DisplayName = "Moving"),
	MidAir			UMETA(DisplayName = "Mid Air"),
	JumpHolding		UMETA(DisplayName = "JumpHolding"),
	Won				UMETA(DisplayName = "Won"),
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

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

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

	UFUNCTION(BlueprintCallable, Category = "Sunny|Recourses")
	void AddCoins(int32 Amount);

	int32 GetCoins() { return Coins; }

	UFUNCTION(BlueprintCallable, Category = "Sunny|Recourses")
	void AddLives(int32 Amount);

	int32 GetLives() { return Lives; }

	UFUNCTION(BlueprintCallable, Category = "Sunny|Recourses")
	void SetHasKey(bool HasKey);

	bool GetHasKey() { return bHasKey; }

	// Use these delegates to update the UI
	UPROPERTY(BlueprintAssignable, Category = "Sunny")
	FOnRecoursesChangedDelegate OnRecoursesChanged;

	UPROPERTY(BlueprintAssignable, Category = "Sunny")
	FOnGameOverDelegate OnGameOver;

protected:
	// Jumping
	UPROPERTY(EditDefaultsOnly, Category = "Sunny|Jump")
	float MaxJumpVelocity; // max height the player should be able to jump

	UPROPERTY(EditDefaultsOnly, Category = "Sunny|Jump")
	float BaseJumpVelocity = 200.f; // multiplier to adjust for increased gravity

	float JumpVelocity; // velocity set in code depending on how long key is held
	
	float JumpKeyDownTime;

	bool bJumpKeyDown;

	float MoveSpeed; // temporary stores move speed variable for stopping movement during jump

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

	void OnTakeDamage();

	void Respawn();

	void Die();

private:
	EPlayerState PS;

	int32 Coins;

	int32 Lives;

	int32 MaxLives;

	bool bHasKey;
};
