// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JCheckpoint.generated.h"



UCLASS()
class IU_JUMPKING_API AJCheckpoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJCheckpoint();

	UPROPERTY(EditAnywhere, Category = "Checkpoint")
	class USkeletalMeshComponent* CheckpointMesh;

	UPROPERTY(EditAnywhere, Category = "Checkpoint")
	class UCapsuleComponent* CollisionComp;

	UPROPERTY(EditAnywhere, Category = "Checkpoint")
	class UArrowComponent* SpawnLocation; 

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// declare overlap begin function
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(EditDefaultsOnly, Category="Checkpoint|FX")
	class USoundBase* CheckpointSound;

	UPROPERTY(EditDefaultsOnly, Category="Checkpoint|FX")
	class UNiagaraSystem* CheckpointEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Checkpoint|FX")
	class UAnimationAsset* CheckpointAnim;

	bool bCheckpointCollected;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CheckpointReachedFX();

};
