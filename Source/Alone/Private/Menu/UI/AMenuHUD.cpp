// Fill out your copyright notice in the Description page of Project Settings.

#include "Menu/UI/AMenuHUD.h"
#include "Menu/UI/AMenuWidget.h"
#include "UI/ABaseWidget.h"

void AAMenuHUD::BeginPlay()
{
    Super::BeginPlay();

    if (MenuWidgetClass)
    {
        const auto MenuWidget = CreateWidget<UABaseWidget>(GetWorld(), MenuWidgetClass);
        if (MenuWidget)
        {
            MenuWidget->AddToViewport();
            MenuWidget->Show();
        }
    }
}
