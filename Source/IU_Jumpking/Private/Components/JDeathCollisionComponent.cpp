// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/JDeathCollisionComponent.h"
#include "Components/CapsuleComponent.h"
#include "Character/JCharacter.h"


// Sets default values for this component's properties
UJDeathCollisionComponent::UJDeathCollisionComponent()
{
	
}

// Called when the game starts
void UJDeathCollisionComponent::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &UJDeathCollisionComponent::OnColliderBeginOverlap);

}

void UJDeathCollisionComponent::OnColliderBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AJCharacter* Player = Cast<AJCharacter>(OtherActor);
	if (Player) {
		Player->Die();
	}

	// !TODO play fx
}