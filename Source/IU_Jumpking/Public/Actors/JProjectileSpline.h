// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/JProjectile.h"
#include "JProjectileSpline.generated.h"

class USplineComponent;
class AJObstacleProjectile;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSplineReceivedDelegate, USplineComponent*, SplineRef);


/**
 * Projectile that receives a spline component in blueprint and moves along it
 */
UCLASS()
class IU_JUMPKING_API AJProjectileSpline : public AJProjectile
{
	GENERATED_BODY()

	AJProjectileSpline();

public:
	UPROPERTY(BlueprintAssignable, Category = "Projectile")
	FOnSplineReceivedDelegate OnSplineReceived;

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SetSplineReference(USplineComponent* Spline);

	UFUNCTION(BlueprintCallable)
	USplineComponent* GetSplineReference() { return SplineReference; };

private:
	USplineComponent* SplineReference;

};
