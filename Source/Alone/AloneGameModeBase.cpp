// Copyright Epic Games, Inc. All Rights Reserved.

#include "AloneGameModeBase.h"
#include "Player/ACharacter.h"
#include "UI/AGameHUD.h"

AAloneGameModeBase::AAloneGameModeBase()
{
    DefaultPawnClass = AACharacter::StaticClass();
    HUDClass = AAGameHUD::StaticClass();
}