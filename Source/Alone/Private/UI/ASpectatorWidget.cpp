// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/ASpectatorWidget.h"
#include "Components/ARespawnComponent.h"
#include "AUtils.h"

bool UASpectatorWidget::GetRespawnTime(int32& RespawnCountDown) const
{
    const auto RespawnComponent = AUtils::GetAPlayerComponent<UARespawnComponent>(GetOwningPlayer());
    if (!RespawnComponent || !RespawnComponent->IsRespawnIsProgress()) return false;

    RespawnCountDown = RespawnComponent->GetRespawnCountDown();
    return true;
}
