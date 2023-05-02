// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/JGoal.h"
#include "Kismet/GameplayStatics.h"
#include "Character/JCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AJGoal::AJGoal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Components
	GoalMesh = CreateDefaultSubobject<UStaticMeshComponent>("Checkpoint Mesh");
	RootComponent = GoalMesh;

	CollisionComp = CreateDefaultSubobject<UCapsuleComponent>("Capsule Collider");
	CollisionComp->SetupAttachment(RootComponent);

	// Dynamics
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AJGoal::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AJGoal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJGoal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJGoal::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AJCharacter* Player = Cast<AJCharacter>(OtherActor);
	if (Player) {
		UE_LOG(LogTemp, Log, TEXT("player reached goal"));
		Player->Win();
		GoalFX();
	}
}

void AJGoal::GoalFX()
{
	// Sound FX
	float RandPitch = FMath::RandRange(.8f, 1.2f); // Random Pitch
	UGameplayStatics::PlaySoundAtLocation(this, GoalSound, GetActorLocation(), 1.f, RandPitch); // !TODO Volume

	// Visual FX
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, GoalEffect, GetActorLocation());
}