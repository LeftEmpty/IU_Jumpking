// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "JDeathColliderComponent.generated.h"

// class UCapsuleComponent;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IU_JUMPKING_API UJDeathColliderComponent : public UCapsuleComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UJDeathColliderComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

		
};
