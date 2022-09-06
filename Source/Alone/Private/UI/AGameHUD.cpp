// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/AGameHUD.h"
#include "Engine/Canvas.h"
#include "UI/ABaseWidget.h"
#include "Alone/AloneGameModeBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogGameHUD, All, All);

void AAGameHUD::DrawHUD()
{
    Super::DrawHUD();

    //DrawCrossHair();
}

void AAGameHUD::BeginPlay()
{
    Super::BeginPlay();

    GameWidgets.Add(EAMatchState::InProgress, CreateWidget<UABaseWidget>(GetWorld(), PlayerHUDWidgetClass));
    GameWidgets.Add(EAMatchState::Pause, CreateWidget<UABaseWidget>(GetWorld(), PauseWidgetClass));
    GameWidgets.Add(EAMatchState::GameOver, CreateWidget<UABaseWidget>(GetWorld(), GameOverWidgetClass));

    for (auto GameWidgetPair : GameWidgets)
    {
        const auto GameWidget = GameWidgetPair.Value;
        if (!GameWidget) continue;

        GameWidget->AddToViewport();
        GameWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    if (GetWorld())
    {
        const auto GameMode = Cast<AAloneGameModeBase>(GetWorld()->GetAuthGameMode());
        if (GameMode)
        {
            GameMode->OnMatchStateChanged.AddUObject(this, &AAGameHUD::OnMatchStateChanged);
        }
    }
}

void AAGameHUD::OnMatchStateChanged(EAMatchState State) 
{
    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
        UE_LOG(LogGameHUD, Display, TEXT("1"));
    }

    if (GameWidgets.Contains(State))
    {
        CurrentWidget = GameWidgets[State];
    }

    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Visible);
        CurrentWidget->Show();
    }

    UE_LOG(LogGameHUD, Display, TEXT("Match state changed: %s"), *UEnum::GetValueAsString(State));
}

void AAGameHUD::DrawCrossHair()
{
    const TInterval<float> Center(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);
    const float HalfLineSize = 10.0f;
    const float LineThickness = 2.0f;
    const FLinearColor LineColor = FLinearColor::Red;

    DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, LineColor, LineThickness);
    DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LineColor, LineThickness);
}

