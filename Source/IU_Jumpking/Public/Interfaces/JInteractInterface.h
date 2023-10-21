// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "JInteractInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UJInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IU_JUMPKING_API IJInteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Player interaction
	UFUNCTION(BlueprintNativeEvent, Category = "Interact")
	void Interact();

	UFUNCTION()
	virtual void InteractPure() = 0;

	// Collision
	UFUNCTION(BlueprintNativeEvent, Category = "Interact")
	void SetNoticed(bool bNoticed);

	UFUNCTION()
	virtual void SetNoticedPure(bool bNoticed) = 0;

	// Player camera movement
	UFUNCTION()
	virtual void StartPlayerCameraLookAtPure() = 0;

	UFUNCTION()
	virtual void EndPlayerCameraLookAtPure() = 0;
};
