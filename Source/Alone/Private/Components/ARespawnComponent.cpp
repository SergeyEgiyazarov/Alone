// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/ARespawnComponent.h"
#include "Alone/AloneGameModeBase.h"

UARespawnComponent::UARespawnComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UARespawnComponent::Respawn(int32 RespawnTime)
{
    if (!GetWorld()) return;

    RespawnCountDown = RespawnTime;
    GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &UARespawnComponent::RespawnTimerUpdate, 1.0f, true);
}

void UARespawnComponent::RespawnTimerUpdate()
{
    if (--RespawnCountDown <= 0)
    {
        if (!GetWorld()) return;
        GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);

        const auto GameMode = Cast<AAloneGameModeBase>(GetWorld()->GetAuthGameMode());
        if (!GameMode) return;

        GameMode->RespawnRequest(Cast<AController>(GetOwner()));
    }
}
