// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JObstacleProjectileSpawner.generated.h"


class UArrowComponent;
class AJProjectile;
class UStaticMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProjectileSpawnedDelegate, AJProjectile*, LastSpawnedProjectile);

UCLASS()
class IU_JUMPKING_API AJObstacleProjectileSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJObstacleProjectileSpawner();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Obstacle", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ObstacleMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Obstacle", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* ArrowComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Obstacle")
	TSubclassOf<AJProjectile> ProjectileClass;

	UFUNCTION(BlueprintCallable, Category = "Obstacle")
	void StopSpawning();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Obstacle")
	float SpawnRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Obstacle")
	float InitialSpawnDelay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Obstacle")
	bool bLoopSpawning;

	UPROPERTY(BlueprintAssignable, Category = "Obstacle")
	FOnProjectileSpawnedDelegate OnProjectileSpawn;

	FTimerHandle TimerHandle;

	virtual void SpawnProjectile();	

};
