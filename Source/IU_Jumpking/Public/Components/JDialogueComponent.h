// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JDialogueComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueUpdatedDelegate, FString, NewPage);


USTRUCT(BlueprintType)
struct FDialogue
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC|Dialogue")
	TArray<FString> DialoguePages;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IU_JUMPKING_API UJDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UJDialogueComponent();

	UPROPERTY(BlueprintAssignable, Category = "NPC|Dialogue")
	FOnDialogueUpdatedDelegate OnDialogueUpdated;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC|Dialogue")
	TArray<FDialogue> Dialogues;

	UFUNCTION(BlueprintCallable, Category = "NPC|Dialogue")
	FString GetPageContent(int PageIndex);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC|Dialogue")
	int32 NumberOfFinishedDialogues;

	void StartDialogue();

	void EndDialoge(bool bAgreed);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


};
