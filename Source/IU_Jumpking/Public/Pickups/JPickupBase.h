// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JPickupBase.generated.h"


class USphereComponent;
class AJCharacter;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPickupCollectedDelegate, AJCharacter*, Player);


UCLASS()
class IU_JUMPKING_API AJPickupBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJPickupBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// declare overlap begin function
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Override depending on Pickup
	UPROPERTY(BlueprintAssignable, Category="Pickup")
	FPickupCollectedDelegate OnPickup;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="Pickup|Compoonents")
	UStaticMeshComponent* PickupMesh;

	UPROPERTY(EditDefaultsOnly, Category="Pickup|Compoonents")
	USphereComponent* CollisionComp;

	UPROPERTY(EditDefaultsOnly, Category="Pickup|FX")
	class USoundBase* PickupSound;

	UPROPERTY(EditDefaultsOnly, Category="Pickup|FX")
	class UNiagaraSystem* PickupEffect;

	UPROPERTY(EditDefaultsOnly, Category="Pickup|Rotation")
	float PitchVal;

	UPROPERTY(EditDefaultsOnly, Category="Pickup|Rotation")
	float YawVal;

	UPROPERTY(EditDefaultsOnly, Category="Pickup|Rotation")
	float RollVal;

	void PickUpFX();
};
