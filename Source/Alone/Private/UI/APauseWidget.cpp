// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/APauseWidget.h"
#include "Gameframework/GameModeBase.h"
#include "Components/Button.h"

bool UAPauseWidget::Initialize()
{
    const auto InitStatus = Super::Initialize();

    if (ClearPauseButton)
    {
        ClearPauseButton->OnClicked.AddDynamic(this, &UAPauseWidget::OnClearPuse);
    }

    return InitStatus;
}

void UAPauseWidget::OnClearPuse()
{
    if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

    GetWorld()->GetAuthGameMode()->ClearPause();
}
