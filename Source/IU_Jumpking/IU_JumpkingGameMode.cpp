// Copyright Epic Games, Inc. All Rights Reserved.

#include "IU_JumpkingGameMode.h"
#include "IU_JumpkingCharacter.h"
#include "UObject/ConstructorHelpers.h"

AIU_JumpkingGameMode::AIU_JumpkingGameMode()
{
	// set default pawn class to our Blueprinted character
	/*
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	*/
}
