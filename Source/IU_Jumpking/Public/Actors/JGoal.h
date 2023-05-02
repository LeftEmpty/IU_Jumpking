// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JGoal.generated.h"

UCLASS()
class IU_JUMPKING_API AJGoal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJGoal();

	UPROPERTY(EditAnywhere, Category = "Checkpoint")
	class UStaticMeshComponent* GoalMesh;

	UPROPERTY(EditAnywhere, Category = "Checkpoint")
	class UCapsuleComponent* CollisionComp;

	// declare overlap begin function
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Checkpoint|FX")
	class USoundBase* GoalSound;

	UPROPERTY(EditDefaultsOnly, Category="Checkpoint|FX")
	class UNiagaraSystem* GoalEffect;
		
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void GoalFX();

};
