// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/JCharacterNPC.h"
#include "Character/JCharacter.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AJCharacterNPC::AJCharacterNPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractWidgetComp"));
	InteractWidget->SetupAttachment(RootComponent);

	bPlayerInRange = false;
}

// Called when the game starts or when spawned
void AJCharacterNPC::BeginPlay()
{
	Super::BeginPlay();

	// Player Controller (for camera movement)
	PC = UGameplayStatics::GetPlayerController(this, 0);

	// Widget
	InteractWidget->SetHiddenInGame(true);
}

// Called every frame
void AJCharacterNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bPlayerInRange) {
		RotateWidgetTowardsPlayer(InteractWidget);
	}
}

// Called to bind functionality to input
void AJCharacterNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AJCharacterNPC::Interact_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("INTERACTED BLUEPRINT"));
}

void AJCharacterNPC::InteractPure()
{
	UE_LOG(LogTemp, Warning, TEXT("INTERACTED PURE"));
	// Interact_Implementation();
}

void AJCharacterNPC::SetNoticed_Implementation(bool bNoticed)
{
	InteractWidget->SetHiddenInGame(!bNoticed);
	bPlayerInRange = bNoticed;
}

void AJCharacterNPC::SetNoticedPure(bool bNoticed)
{
	InteractWidget->SetHiddenInGame(!bNoticed);
	bPlayerInRange = bNoticed;
}

void AJCharacterNPC::StartDialog()
{
	
}

void AJCharacterNPC::RotateWidgetTowardsPlayer(UWidgetComponent* Widget)
{
	APlayerCameraManager* PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	if (PlayerCameraManager) {
		FVector WidgetLoc = Widget->GetComponentLocation();
		FVector CameraLoc = PlayerCameraManager->GetCameraLocation();
		Widget->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(WidgetLoc, CameraLoc));
	}
}

void AJCharacterNPC::StartPlayerCameraLookAtPure()
{
	if (PC && LookAtTarget) {
		PC->SetViewTargetWithBlend(LookAtTarget, BlendTime);
	}
}

void AJCharacterNPC::EndPlayerCameraLookAtPure()
{
	if (PC && LookAtTarget) {
		PC->SetViewTargetWithBlend(PC->GetPawn(), BlendTime);
	}
}