// Fill out your copyright notice in the Description page of Project Settings.

#include "Menu/UI/AMenuHUD.h"
#include "Menu/UI/AMenuWidget.h"

void AAMenuHUD::BeginPlay()
{
    Super::BeginPlay();

    const auto MenuWidget = CreateWidget<UAMenuWidget>(GetWorld(), MenuWidgetClass);
    if (MenuWidget)
    {
        MenuWidget->AddToViewport();
    }
}
