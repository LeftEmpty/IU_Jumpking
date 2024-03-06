// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/JDeathColliderComponent.h"
#include "Components/CapsuleComponent.h"


// Sets default values for this component's properties
UJDeathColliderComponent::UJDeathColliderComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UJDeathColliderComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

