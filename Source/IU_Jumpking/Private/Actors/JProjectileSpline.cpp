// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/JProjectileSpline.h"
#include "Actors/JProjectile.h"
#include "Components/SplineComponent.h"
#include "Components/ArrowComponent.h"


AJProjectileSpline::AJProjectileSpline()
{

}

void AJProjectileSpline::SetSplineReference(USplineComponent* Spline)
{
	SplineReference = Spline;
	OnSplineReceived.Broadcast(Spline);
}