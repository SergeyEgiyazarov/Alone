// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ACoreTypes.h"
#include "AloneGameModeBase.generated.h"

class AAIController;

UCLASS()
class ALONE_API AAloneGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    AAloneGameModeBase();

    FOnMatchStateChangedSignature OnMatchStateChanged;

    virtual void StartPlay() override;
    virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

    void Killed(AController* KillerController, AController* VictimController);
    int32 GetCurrentRound() { return CurrentRound; }
    FGameData& GetGameData() { return GameData; }
    int32 GetRoundSecondRemaining() { return RoundCountDown; }

    void RespawnRequest(AController* Controller);

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSubclassOf<AAIController> AIControllerClass;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSubclassOf<APawn> AIPawnClass;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FGameData GameData;

private:
    EAMatchState MatchState = EAMatchState::WaitingToStart;
    int32 CurrentRound = 1;
    int32 RoundCountDown = 0;
    FTimerHandle GameRoundTimerHandle;

    void SpawnBots();
    void StartRound();
    void GameTimerUpdate();

    void ResetPlayers();
    void ResetOnePlayer(AController* Controller);

    void CreateTeamsInfo();
    FLinearColor DetermineColorByTeamID(int32 ID) const;
    void SetPlayerColor(AController* Controller);

    void StartRespawn(AController* Controller);
    void GameOver();

    void SetMatchState(EAMatchState State);

    void LogPlayerInfo();
};
