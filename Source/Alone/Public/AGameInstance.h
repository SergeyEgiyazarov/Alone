// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ACoreTypes.h"
#include "AGameInstance.generated.h"

UCLASS()
class ALONE_API UAGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    FLevelData GetStartupLevel() const { return StartupLevel; }
    void SetStartupLevel(const FLevelData& Data) { StartupLevel = Data; }

    const TArray<FLevelData>& GetLevelsData() const { return LevelsData; }

    FName GetMainMenuLevelName() const { return MainMenuLevelName; }

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TArray<FLevelData> LevelsData;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FName MainMenuLevelName = NAME_None;

private:
    FLevelData StartupLevel;
};
