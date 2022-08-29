// Copyright Epic Games, Inc. All Rights Reserved.

#include "AloneGameModeBase.h"
#include "Player/ACharacter.h"
#include "UI/AGameHUD.h"
#include "AIController.h"

AAloneGameModeBase::AAloneGameModeBase()
{
    DefaultPawnClass = AACharacter::StaticClass();
    HUDClass = AAGameHUD::StaticClass();
}

void AAloneGameModeBase::StartPlay()
{
    Super::StartPlay();

    SpawnBots();
    CurrentRound = 1;
    StartRound();
}

UClass* AAloneGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
    if (InController && InController->IsA<AAIController>())
    {
        return AIPawnClass;
    }
    return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void AAloneGameModeBase::SpawnBots()
{
    if (!GetWorld()) return;

    for (int32 i = 0; i < GameData.PlayersNum - 1; ++i)
    {
        FActorSpawnParameters SpawnInfo;
        SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        const auto AIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);
        RestartPlayer(AIController);
    }
}

void AAloneGameModeBase::StartRound()
{
    RoundCountDown = GameData.RoundTime;
    GetWorldTimerManager().SetTimer(GameRoundTimerHandle, this, &AAloneGameModeBase::GameTimerUpdate, 1.0f, true);
}

void AAloneGameModeBase::GameTimerUpdate()
{
    if (--RoundCountDown <= 0)
    {
        GetWorldTimerManager().ClearTimer(GameRoundTimerHandle);

        if (CurrentRound + 1 <= GameData.RoundNum)
        {
            ++CurrentRound;
            StartRound();
        }
        else
        {
            // GameOver
        }
    }
}
