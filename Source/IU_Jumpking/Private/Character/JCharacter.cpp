// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/JCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


// Sets default values
AJCharacter::AJCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(142.f, 80.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = JumpVelocity;
	GetCharacterMovement()->AirControl = 0.f; // for the "jumpking" feel
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 600.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Default values
	MaxJumpVelocity = 2000.f;
	JumpVelocity = 0.f;
	JumpKeyDownTime = 0.f;
	bJumpKeyDown = false;

	MaxLives = 3;
	Lives = MaxLives;

	Coins = 0;

	bHasKey = false;
}

// Called when the game starts or when spawned
void AJCharacter::BeginPlay()
{
	Super::BeginPlay();

	MoveSpeed = GetCharacterMovement()->MaxWalkSpeed;
	
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

// Called every frame
void AJCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Add jump force if jump key is held
	if (bJumpKeyDown) {
		// !TODO this is hard coded to space bar -> fix to make it catch all input "jumpaction" key inputs
		// FKey JumpKey = GetWorld()->GetFirstPlayerController()->PlayerInput->GetKeysForAction(FName("JumpAction")); doesnt work
		JumpKeyDownTime = FMath::Clamp(GetWorld()->GetFirstPlayerController()->GetInputKeyTimeDown(FKey("SpaceBar")),.35f, 1.f);
		FString tmp3 = FString::SanitizeFloat(JumpKeyDownTime);
		GEngine->AddOnScreenDebugMessage(4, 10.f, FColor::Cyan, *tmp3, true);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AJCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AJCharacter::JumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AJCharacter::JumpEnd);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AJCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AJCharacter::Look);

	}
}

void AJCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr) {
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AJCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr) {
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AJCharacter::JumpStart()
{
	// States
	bJumpKeyDown = true;
	SetPlayerState(EPlayerState::JumpHolding);

	// Fix Movement
	GetCharacterMovement()->MaxWalkSpeed = 0.0f;
}

void AJCharacter::JumpEnd()
{
	// States
	bJumpKeyDown = false;
	SetPlayerState(EPlayerState::MidAir);

	// Reset Movement
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;

	// Set Jump Height
	JumpVelocity = MaxJumpVelocity * JumpKeyDownTime + BaseJumpVelocity;
	GetCharacterMovement()->JumpZVelocity = JumpVelocity;

	// Debug
	FString tmp = FString::SanitizeFloat(JumpKeyDownTime);
	GEngine->AddOnScreenDebugMessage(4, 10.f, FColor::Blue, *tmp, true);

	FString tmp2 = FString::SanitizeFloat(JumpVelocity);
	GEngine->AddOnScreenDebugMessage(5, 10.f, FColor::Purple, *tmp2, true);
	
	float JumpForward = 200 + JumpKeyDownTime * 500;
	FVector LaunchVector = FVector(GetActorForwardVector().X* JumpForward, GetActorForwardVector().Y* JumpForward, JumpVelocity);
	LaunchCharacter(LaunchVector, false, false);
}

void AJCharacter::OnTakeDamage()
{
	if (Lives > 1) {
		AddLives(-1);
		// Respawn

	}
	else {
		// Game Over
		Die();
	}
}

void AJCharacter::Respawn()
{
	// !TODO respawn player at start / last checkpoint
}

void AJCharacter::Die()
{
	// !TODO change game state & cause gm to show game over screen
	OnGameOver.Broadcast(this);
}

void AJCharacter::SetPlayerState(EPlayerState NewState)
{
	PS = NewState;
}

void AJCharacter::AddCoins(int32 Amount)
{
	// !TODO Add Lives when reaching 99 coins and reset to 0
	Coins = FMath::Clamp(Coins + Amount, 0, 99);
	OnRecoursesChanged.Broadcast(this, Coins, Lives, bHasKey);
	GEngine->AddOnScreenDebugMessage(300, 10.f, FColor::Yellow, FString::Printf(TEXT("Coins: %s"), *FString::FromInt(Coins)));
}

void AJCharacter::AddLives(int32 Amount)
{
	Lives = FMath::Clamp(Lives + Amount, 0, MaxLives);
	OnRecoursesChanged.Broadcast(this, Coins, Lives, bHasKey);
	GEngine->AddOnScreenDebugMessage(301, 10.f, FColor::Red, FString::Printf(TEXT("Lives: %s"), *FString::FromInt(Lives)));
}

void AJCharacter::SetHasKey(bool HasKey)
{
	bHasKey = HasKey;
	OnRecoursesChanged.Broadcast(this, Coins, Lives, bHasKey);
	GEngine->AddOnScreenDebugMessage(302, 10.f, FColor::Orange, FString::Printf(TEXT("Key: %s"), HasKey ? TEXT("true") : TEXT("false")));
}



