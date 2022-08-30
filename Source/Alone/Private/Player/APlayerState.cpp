// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/APlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(LogAPlayerState, All, All);

void AAPlayerState::LogInfo()
{
    UE_LOG(LogAPlayerState, Display, TEXT("TeamID: %i, Kills: %i, Deaths: %i"), TeamID, KillsNum, DeathsNum);
}
