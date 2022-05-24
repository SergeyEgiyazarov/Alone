// Copyright Epic Games, Inc. All Rights Reserved.


#include "AloneGameModeBase.h"
#include "Player/ACharacter.h"

AAloneGameModeBase::AAloneGameModeBase()
{
	DefaultPawnClass = AACharacter::StaticClass();
}