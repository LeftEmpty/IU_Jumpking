// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/JCharacter.h"

#include "Camera/CameraComponent.h"

#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"

#include "Interfaces/JInteractInterface.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"

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
	GetCharacterMovement()->JumpZVelocity = JumpHeightVelocity;
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

	// Interact Collision
	CapsuleColliderComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("InteractColliderComp"));
	CapsuleColliderComp->SetupAttachment(RootComponent);
	CapsuleColliderComp->SetCapsuleHalfHeight(420.f);
	CapsuleColliderComp->SetCapsuleRadius(340.f);
	CapsuleColliderComp->SetRelativeLocation(FVector(80.f,0,20.f));
	CapsuleColliderComp->SetRelativeRotation(FRotator(-90,0,0));


	// Default values
	MaxJumpHeightVelocity = 2000.f;
	JumpHeightVelocity = 0.f;
	JumpKeyDownTime = 0.f;
	bJumpKeyDown = false;

	MaxLives = 3;
	Lives = MaxLives;

	Coins = 0;

	bHasKey = false;
	bCheckpointReached = false;

	bDead = false;
}

// Called when the game starts or when spawned
void AJCharacter::BeginPlay()
{
	Super::BeginPlay();

	CapsuleColliderComp->OnComponentBeginOverlap.AddDynamic(this, &AJCharacter::OnInteractableNoticed);
	CapsuleColliderComp->OnComponentEndOverlap.AddDynamic(this, &AJCharacter::OnInteractableForgotten);

	MoveSpeed = GetCharacterMovement()->MaxWalkSpeed;
	
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Set Player Start Location
	PlayerStartVec = GetActorLocation();
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

		//Interacting
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AJCharacter::Interact);
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
	if (GetCharacterMovement()->IsFalling()) {
		return;
	}

	// States
	bJumpKeyDown = true;
	SetPlayerState(EPlayerState::JumpHolding);

	// Fix Movement
	GetCharacterMovement()->MaxWalkSpeed = 0.0f;
}

void AJCharacter::JumpEnd()
{
	if (GetCharacterMovement()->IsFalling()) {
		return;
	}

	// States
	bJumpKeyDown = false;
	SetPlayerState(EPlayerState::MidAir);

	// Reset Movement
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;

	// Set Jump Height
	JumpHeightVelocity = MaxJumpHeightVelocity * JumpKeyDownTime + BaseJumpHeightVelocity;
	// GetCharacterMovement()->JumpZVelocity = JumpHeightVelocity;

	// Debug
	FString tmp = FString::SanitizeFloat(JumpKeyDownTime);
	GEngine->AddOnScreenDebugMessage(4, 10.f, FColor::Blue, *tmp, true);

	FString tmp2 = FString::SanitizeFloat(JumpHeightVelocity);
	GEngine->AddOnScreenDebugMessage(5, 10.f, FColor::Purple, *tmp2, true);
	
	float JumpForward = BaseJumpForwardVelocity + JumpKeyDownTime * JumpForwardVelocity;
	FVector LaunchVector = FVector(GetActorForwardVector().X * JumpForward, GetActorForwardVector().Y * JumpForward, JumpHeightVelocity);
	LaunchCharacter(LaunchVector, false, false);
}

void AJCharacter::SetPlayerState(EPlayerState NewState)
{
	PS = NewState;
}

void AJCharacter::AddCoins(int32 Amount)
{
	// !TODO Add Lives when reaching 99 coins and reset to 0 ?
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

void AJCharacter::Die()
{
	if (bDead) {
		return;
	}
	bDead = true;
	
	// Hide Mesh
	GetMesh()->SetVisibility(false);
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->MaxWalkSpeed = 0.0f;
	GetCharacterMovement()->GravityScale = 0.0f;

	// FX
	DeathFX();
		
	// Respawn
	if (Lives > 1) {
		OnDeath.Broadcast(this, false);
		AddLives(-1);
		GetWorld()->GetTimerManager().SetTimer(FOnDeathTimer, this, &AJCharacter::Respawn, 1.25f, false);
	}
	// Game over
	else {
		OnDeath.Broadcast(this, true);
		GameOver();
		GetWorld()->GetTimerManager().SetTimer(FOnDeathTimer, this, &AJCharacter::GameOver, 1.25f, false);
	}
}

void AJCharacter::GameOver()
{
	UE_LOG(LogTemp, Log, TEXT("game over"));

	// Clear Timer
	GetWorld()->GetTimerManager().ClearTimer(FOnDeathTimer);
		
	this->SetLifeSpan(2.5f);
}

void AJCharacter::Respawn()
{
	UE_LOG(LogTemp, Log, TEXT("repsawn"));
	
	bDead = false;

	// Reset gravity (defauult value 2.0 as of now)
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->GravityScale = 2.0f;

	// Reset Mesh Visibility
	GetMesh()->SetVisibility(true);

	// Move to last Checkpoint or Start
	if (bCheckpointReached) {
		SetActorLocation(LastCheckpointLoc + FVector(0, 0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()), false);
	}
	else {
		SetActorLocation(PlayerStartVec + FVector(0, 0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()), false);
	}

	// Clear Timer
	GetWorld()->GetTimerManager().ClearTimer(FOnDeathTimer);
}

void AJCharacter::DeathFX()
{
	// Set Mesh Visibility
	GetMesh()->SetVisibility(false);

	// Sound FX
	float RandPitch = FMath::RandRange(.8f, 1.2f); // Random Pitch
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), 1.f, RandPitch); // !TODO Volume

	// Visual FX
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, DeathEffect, GetActorLocation());
}

void AJCharacter::UpdateCheckpoint(FVector CheckpointSpawnLocation)
{
	bCheckpointReached = true;
	LastCheckpointLoc = CheckpointSpawnLocation;
	UE_LOG(LogTemp, Log, TEXT("Checkpoint set"));
}

void AJCharacter::Win() {
	// Player State
	SetPlayerState(EPlayerState::Won);
	
	// Delegate to update UI
	OnVictory.Broadcast(this);

	// Disable Movement
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();

	// FX
	PlayAnimMontage(VictoryAnimation, 1.0f, "start_1");
}

void AJCharacter::ChangeToLevel(FString Level)
{
	if (Levels.Contains(Level)) {
		UGameplayStatics::OpenLevel(GetWorld(), FName(Level));
	}
}

void AJCharacter::OnInteractableNoticed(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IJInteractInterface* Interactable = Cast<IJInteractInterface>(OtherActor);
	if (OtherActor && Interactable) {
		InteractablesInRange.Add(Interactable);
		Interactable->SetNoticedPure(true);
		Interactable->Execute_SetNoticed(OtherActor, true);
	}
}

void AJCharacter::OnInteractableForgotten(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IJInteractInterface* Interactable = Cast<IJInteractInterface>(OtherActor);
	if (OtherActor && Interactable) {
		if (InteractablesInRange.Contains(Interactable)) {
			InteractablesInRange.Remove(Interactable);
		}
		Interactable->SetNoticedPure(false);
		Interactable->Execute_SetNoticed(OtherActor, false);
	}
}

void AJCharacter::Interact()
{
	if (InteractablesInRange.Num() > 0 && InteractablesInRange[0]) {
		// Call to interaface
		InteractablesInRange[0]->InteractPure();
		// Camera
		InteractablesInRange[0]->StartPlayerCameraLookAtPure();
		// Rotate player towards interactable
		AActor* I = Cast<AActor>(InteractablesInRange[0]);
		if (I) {
			SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), I->GetActorLocation()));
			// !TODO stop input (ALSO RESET FUNCTION)
		}
	}
}

