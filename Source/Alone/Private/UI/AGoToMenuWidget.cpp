// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/AGoToMenuWidget.h"
#include "AGameInstance.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogGoToMenuWidget, All, All);

void UAGoToMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (MainMenuButton)
    {
        MainMenuButton->OnClicked.AddDynamic(this, &UAGoToMenuWidget::GoToMainMenu);
    }
}

void UAGoToMenuWidget::GoToMainMenu()
{
    if (!GetWorld()) return;

    const auto GameInstance = GetWorld()->GetGameInstance<UAGameInstance>();
    if (!GameInstance) return;

    if (GameInstance->GetMainMenuLevelName().IsNone())
    {
        UE_LOG(LogGoToMenuWidget, Error, TEXT("Main Menu Level name is none"));
        return;
    }

    UGameplayStatics::OpenLevel(this, GameInstance->GetMainMenuLevelName());
}
