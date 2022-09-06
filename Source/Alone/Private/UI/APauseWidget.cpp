// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/APauseWidget.h"
#include "Gameframework/GameModeBase.h"
#include "Components/Button.h"

void UAPauseWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (ClearPauseButton)
    {
        ClearPauseButton->OnClicked.AddDynamic(this, &UAPauseWidget::OnClearPuse);
    }
}

void UAPauseWidget::OnClearPuse()
{
    if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

    GetWorld()->GetAuthGameMode()->ClearPause();
}
