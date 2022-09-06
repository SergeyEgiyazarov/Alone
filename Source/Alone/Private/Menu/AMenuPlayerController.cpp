// Fill out your copyright notice in the Description page of Project Settings.

#include "Menu/AMenuPlayerController.h"

void AAMenuPlayerController::BeginPlay()
{
    Super::BeginPlay();

    SetInputMode(FInputModeUIOnly());
    bShowMouseCursor = true;
}
