// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/JProjectile.h"
#include "Character/JCharacter.h"
#include "Components/JDeathCollisionComponent.h"
#include "Components/ArrowComponent.h"


// Sets default values
AJProjectile::AJProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow Comp"));
	RootComponent = ArrowComp;

	CollisionComp = CreateDefaultSubobject<UJDeathCollisionComponent>(TEXT("Death Collision Comp"));
	CollisionComp->SetupAttachment(RootComponent);

	LifeSpan = 5.f;
}

// Called when the game starts or when spawned
void AJProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LifeSpan);
}

