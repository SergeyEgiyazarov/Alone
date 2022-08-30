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

public:
    void SetTeamID(int32 ID) { TeamID = ID; }
    int32 GetTeamID() const { return TeamID; }

    void SetTeamColor(const FLinearColor& Color) { TeamColor = Color; }
    FLinearColor GetTeamColor() const { return TeamColor; }
};
