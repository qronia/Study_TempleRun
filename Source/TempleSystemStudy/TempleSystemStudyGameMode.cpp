// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TempleSystemStudyGameMode.h"
#include "TempleSystemStudyCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATempleSystemStudyGameMode::ATempleSystemStudyGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/RunCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}