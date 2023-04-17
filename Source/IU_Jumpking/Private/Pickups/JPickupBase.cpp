// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/JPickupBase.h"
#include "Character/JCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AJPickupBase::AJPickupBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Components
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Component"));
	RootComponent = CollisionComp;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup Mesh"));
	PickupMesh->SetupAttachment(RootComponent);
	
	// Dynamics
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AJPickupBase::OnOverlapBegin);

	// Set Tick Rate
	SetActorTickInterval(0.2);

	// Default Values
	PitchVal = 0.f;
	YawVal = 25.f;
	RollVal = 0.f;
}

// Called when the game starts or when spawned
void AJPickupBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJPickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator NewRot = FRotator(PitchVal, YawVal, RollVal);
	FQuat QuatRot = FQuat(NewRot);

	AddActorLocalRotation(QuatRot, false);
}

void AJPickupBase::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AJCharacter* PlayerChar = Cast<AJCharacter>(OtherActor);
	if (PlayerChar && (OtherActor != this) && OtherComp) {
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Overlap Begin")); // debug
		OnPickup.Broadcast(PlayerChar);
		PickUpFX();
		Destroy();
	}
}

void AJPickupBase::PickUpFX()
{
	// Sound FX
	float RandPitch = FMath::RandRange(.8f, 1.2f); // Random Pitch
	UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation(), 1.f, RandPitch); // !TODO Volume

	// Visual FX
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, PickupEffect, GetActorLocation());
}

