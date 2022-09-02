// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/APlayerController.h"
#include "Components/ARespawnComponent.h"

AAPlayerController::AAPlayerController()
{
    RespawnComponent = CreateDefaultSubobject<UARespawnComponent>("RespawnComponent");
}