// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/JInteractInterface.h"
#include "JCharacterNPC.generated.h"


class UWidgetComponent;


UCLASS()
class IU_JUMPKING_API AJCharacterNPC : public ACharacter, public IJInteractInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AJCharacterNPC();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC")
	UWidgetComponent* InteractWidget;

	// Interaction Interface
	void Interact_Implementation() override;

	virtual void InteractPure() override;

	void SetNoticed_Implementation(bool bNoticed) override;

	virtual void SetNoticedPure(bool bNoticed) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void StartDialog();

	// Camera Control
	UPROPERTY(EditAnywhere, Category = "NPC")
	float BlendTime = 1.0f;

	virtual void StartPlayerCameraLookAtPure() override;

	virtual void EndPlayerCameraLookAtPure() override;
	
	UPROPERTY(EditAnywhere, Category = "NPC")
	AActor* LookAtTarget;

private:
	bool bPlayerInRange;

	APlayerController* PC;

	void RotateWidgetTowardsPlayer(UWidgetComponent* Widget);
};
