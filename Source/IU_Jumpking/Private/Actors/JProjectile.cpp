// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/JProjectile.h"
#include "Character/JCharacter.h"

#include "Components/JDeathColliderComponent.h"
#include "Components/ArrowComponent.h"


// Sets default values
AJProjectile::AJProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow Comp"));
	RootComponent = ArrowComp;

	ColliderComp = CreateDefaultSubobject<UJDeathColliderComponent>(TEXT("ColliderComp"));
	ColliderComp->SetupAttachment(RootComponent);

	LifeSpan = 5.f;
}

// Called when the game starts or when spawned
void AJProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	//ColliderComp->OnComponentBeginOverlap.AddDynamic(this, &AJProjectile::OnColliderBeginOverlap);

	SetLifeSpan(LifeSpan);
}

void AJProjectile::OnColliderBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AJCharacter* Player = Cast<AJCharacter>(OtherActor);
	if (Player) {
		Player->Die();
	}
	// !TODO play fx
}


