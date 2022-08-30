// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "APlayerState.generated.h"

UCLASS()
class ALONE_API AAPlayerState : public APlayerState
{
    GENERATED_BODY()

private:
    int32 TeamID;
    FLinearColor TeamColor;

    int32 KillsNum = 0;
    int32 DeathsNum = 0;

public:
    void SetTeamID(int32 ID) { TeamID = ID; }
    int32 GetTeamID() const { return TeamID; }

    void SetTeamColor(const FLinearColor& Color) { TeamColor = Color; }
    FLinearColor GetTeamColor() const { return TeamColor; }

    void AddKill() { ++KillsNum; }
    int32 GetKills() { return KillsNum; }

    void AddDeath() { ++DeathsNum; }
    int32 GetDeaths() { return DeathsNum; }

    void LogInfo();
};
