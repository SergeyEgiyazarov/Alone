// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/AGameHUD.h"
#include "Engine/Canvas.h"
#include "Blueprint/UserWidget.h"
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

    const auto PlayerHUD = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass);
    if (PlayerHUD)
    {
        PlayerHUD->AddToViewport();
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

