// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/AGameDataWidget.h"
#include "Alone/AloneGameModeBase.h"
#include "Player/APlayerState.h"

int32 UAGameDataWidget::GetKillsNum() const
{
    const auto PlayerState = GetAPlayerState();
    return PlayerState ? PlayerState->GetKills() : 0;
}

int32 UAGameDataWidget::GetCurrentRoundNum() const
{
    const auto GameMode = GetAGameModeBase();
    return GameMode ? GameMode->GetCurrentRound() : 0;
}

int32 UAGameDataWidget::GetTotalRoundsNum() const
{
    const auto GameMode = GetAGameModeBase();
    return GameMode ? GameMode->GetGameData().RoundNum : 0;
}

int32 UAGameDataWidget::GetRoundSecondRemaining() const
{
    const auto GameMode = GetAGameModeBase();
    return GameMode ? GameMode->GetRoundSecondRemaining() : 0;
}

AAloneGameModeBase* UAGameDataWidget::GetAGameModeBase() const
{
    return GetWorld() ? Cast<AAloneGameModeBase>(GetWorld()->GetAuthGameMode()) : nullptr;
}

AAPlayerState* UAGameDataWidget::GetAPlayerState() const
{
    return GetOwningPlayer() ? Cast<AAPlayerState>(GetOwningPlayer()->PlayerState) : nullptr;
}
