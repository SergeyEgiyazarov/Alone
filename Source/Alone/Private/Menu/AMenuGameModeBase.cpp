// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu/AMenuGameModeBase.h"
#include "Menu/AMenuPlayerController.h"
#include "Menu/UI/AMenuHUD.h"

AAMenuGameModeBase::AAMenuGameModeBase()
{
    PlayerControllerClass = AAMenuPlayerController::StaticClass();
    HUDClass = AAMenuHUD::StaticClass();
}
