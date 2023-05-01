// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/JCheckpoint.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Character/JCharacter.h"


// Sets default values
AJCheckpoint::AJCheckpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Components
	CheckpointMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Checkpoint Mesh");
	RootComponent = CheckpointMesh;

	CollisionComp = CreateDefaultSubobject<UCapsuleComponent>("Capsule Collider");
	CollisionComp->SetupAttachment(RootComponent);

	SpawnLocation = CreateDefaultSubobject<UArrowComponent>("Spawn Location");
	SpawnLocation->SetupAttachment(RootComponent);
	SpawnLocation->ArrowColor = FColor::Emerald;
	SpawnLocation->ArrowSize = 1.5f;

	// Dynamics
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AJCheckpoint::OnOverlapBegin);

}

// Called when the game starts or when spawned
void AJCheckpoint::BeginPlay()
{
	Super::BeginPlay();
	
}



// Called every frame
void AJCheckpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJCheckpoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bCheckpointCollected) {
		return;
	}
	AJCharacter* Player = Cast<AJCharacter>(OtherActor);
	if (Player) {
		bCheckpointCollected = true;
		Player->UpdateCheckpoint(SpawnLocation->GetComponentLocation());
		CheckpointReachedFX();
	}
}

void AJCheckpoint::CheckpointReachedFX()
{
	// Sound FX
	if (CheckpointSound) {
		float RandPitch = FMath::RandRange(.8f, 1.2f); // Random Pitch
		UGameplayStatics::PlaySoundAtLocation(this, CheckpointSound, GetActorLocation(), 1.f, RandPitch); // !TODO Volume
	}
	
	// Visual FX
	if (CheckpointEffect) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, CheckpointEffect, GetActorLocation() + FVector(0,0,250.f));
	}

	// Anim
	if (CheckpointAnim) {
		CheckpointMesh->PlayAnimation(CheckpointAnim, false);
	}
}
