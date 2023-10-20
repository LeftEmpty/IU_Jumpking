// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/JObstacleProjectileSpawner.h"
#include "Actors/JProjectile.h"
#include "Components/ArrowComponent.h"


// Sets default values
AJObstacleProjectileSpawner::AJObstacleProjectileSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ObstacleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Obstacle Mesh"));
	RootComponent = ObstacleMesh;

	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow Comp"));
	ArrowComp->SetupAttachment(RootComponent);

	SpawnRate = 2.5f;
	InitialSpawnDelay = SpawnRate;
	bLoopSpawning = true;
}


// Called when the game starts or when spawned
void AJObstacleProjectileSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	// Timer to Spawn Projectiles, Movement is done via respective Projectile classes
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AJObstacleProjectileSpawner::SpawnProjectile,
	SpawnRate, bLoopSpawning, InitialSpawnDelay);
}

void AJObstacleProjectileSpawner::SpawnProjectile()
{
	FActorSpawnParameters Params;

	AJProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AJProjectile>(ProjectileClass, ArrowComp->GetComponentLocation(), ArrowComp->GetComponentRotation(), Params);

	OnProjectileSpawn.Broadcast(SpawnedProjectile);
}

void AJObstacleProjectileSpawner::StopSpawning()
{
	GetWorldTimerManager().ClearTimer(TimerHandle);
}

