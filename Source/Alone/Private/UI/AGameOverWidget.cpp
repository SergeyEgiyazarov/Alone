// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/AGameOverWidget.h"
#include "Alone/AloneGameModeBase.h"
#include "Player/APlayerState.h"
#include "UI/APlayerStatRowWidget.h"
#include "Components/VerticalBox.h"
#include "AUtils.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UAGameOverWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (GetWorld())
    {
        const auto GameMode = Cast<AAloneGameModeBase>(GetWorld()->GetAuthGameMode());
        if (GameMode)
        {
            GameMode->OnMatchStateChanged.AddUObject(this, &UAGameOverWidget::OnMatchStateChanged);
        }
    }

    if (ResetLevelButton)
    {
        ResetLevelButton->OnClicked.AddDynamic(this, &UAGameOverWidget::OnResetLevel);
    }
}

void UAGameOverWidget::OnMatchStateChanged(EAMatchState State)
{
    if (State == EAMatchState::GameOver)
    {
        UpdatePlayerStat();
    }
}

void UAGameOverWidget::UpdatePlayerStat()
{
    if (!GetWorld() || !PlayerStatBox) return;
    PlayerStatBox->ClearChildren();

    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto Controller = It->Get();
        if (!Controller) continue;

        const auto PlayerState = Cast<AAPlayerState>(Controller->PlayerState);
        if (!PlayerState) continue;

        const auto PlayerStatRowWidget = CreateWidget<UAPlayerStatRowWidget>(GetWorld(), PlayerStatRowWidgetClass);
        if (!PlayerStatRowWidget) continue;

        PlayerStatRowWidget->SetPlayerName(FText::FromString(PlayerState->GetPlayerName()));
        PlayerStatRowWidget->SetKills(AUtils::TextFromInt(PlayerState->GetKills()));
        PlayerStatRowWidget->SetDeaths(AUtils::TextFromInt(PlayerState->GetDeaths()));
        PlayerStatRowWidget->SetTeam(AUtils::TextFromInt(PlayerState->GetTeamID()));
        PlayerStatRowWidget->SetPlayerIndicatorVisibility(Controller->IsPlayerController());
        PlayerStatRowWidget->SetTeamColor(PlayerState->GetTeamColor());

        PlayerStatBox->AddChild(PlayerStatRowWidget);
    }
}

void UAGameOverWidget::OnResetLevel()
{
    const FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
    UGameplayStatics::OpenLevel(this, FName(CurrentLevelName));
}
