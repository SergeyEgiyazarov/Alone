// Copyright Epic Games, Inc. All Rights Reserved.

#include "AloneGameModeBase.h"
#include "Player/ACharacter.h"
#include "Player/APlayerController.h"
#include "UI/AGameHUD.h"
#include "AIController.h"
#include "Player/APlayerState.h"
#include "Components/ARespawnComponent.h"
#include "AUtils.h"
#include "EngineUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogAGameModeBase, All, All);

constexpr int32 MinRoundTimeForRespawn = 10;

AAloneGameModeBase::AAloneGameModeBase()
{
    DefaultPawnClass = AACharacter::StaticClass();
    PlayerControllerClass = AAPlayerController::StaticClass();
    HUDClass = AAGameHUD::StaticClass();
    PlayerStateClass = AAPlayerState::StaticClass();
}

void AAloneGameModeBase::StartPlay()
{
    Super::StartPlay();

    SpawnBots();
    CreateTeamsInfo();
    CurrentRound = 1;
    StartRound();

    SetMatchState(EAMatchState::InProgress);
}

UClass* AAloneGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
    if (InController && InController->IsA<AAIController>())
    {
        return AIPawnClass;
    }
    return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void AAloneGameModeBase::SpawnBots()
{
    if (!GetWorld()) return;

    for (int32 i = 0; i < GameData.PlayersNum - 1; ++i)
    {
        FActorSpawnParameters SpawnInfo;
        SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        const auto AIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);
        RestartPlayer(AIController);
    }
}

void AAloneGameModeBase::StartRound()
{
    RoundCountDown = GameData.RoundTime;
    GetWorldTimerManager().SetTimer(GameRoundTimerHandle, this, &AAloneGameModeBase::GameTimerUpdate, 1.0f, true);
}

void AAloneGameModeBase::GameTimerUpdate()
{
    if (--RoundCountDown <= 0)
    {
        GetWorldTimerManager().ClearTimer(GameRoundTimerHandle);

        if (CurrentRound + 1 <= GameData.RoundNum)
        {
            ++CurrentRound;
            ResetPlayers();
            StartRound();
        }
        else
        {
            GameOver();
        }
    }
}

void AAloneGameModeBase::ResetPlayers()
{
    if (!GetWorld()) return;

    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        ResetOnePlayer(It->Get());
    }
}

void AAloneGameModeBase::ResetOnePlayer(AController* Controller)
{
    if (Controller && Controller->GetPawn())
    {
        Controller->GetPawn()->Reset();
    }
    RestartPlayer(Controller);
    SetPlayerColor(Controller);
}

void AAloneGameModeBase::CreateTeamsInfo()
{
    if (!GetWorld()) return;

    int32 TeamID = 1;
    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto Controller = It->Get();
        if (!Controller) continue;

        const auto PlayerState = Cast<AAPlayerState>(Controller->PlayerState);
        if (!PlayerState) continue;

        PlayerState->SetTeamID(TeamID);
        PlayerState->SetTeamColor(DetermineColorByTeamID(TeamID));
        SetPlayerColor(Controller);

        TeamID = TeamID == 1 ? 2 : 1;
    }
}

FLinearColor AAloneGameModeBase::DetermineColorByTeamID(int32 ID) const
{
    if (ID - 1 < GameData.TeamColors.Num())
    {
        return GameData.TeamColors[ID - 1];
    }
    UE_LOG(LogAGameModeBase, Warning, TEXT("No color for team id: %i, set default: %s"), ID, *GameData.DefaultTeamColor.ToString());
    return GameData.DefaultTeamColor;
}

void AAloneGameModeBase::SetPlayerColor(AController* Controller)
{
    if (!Controller) return;

    const auto Character = Cast<AACharacter>(Controller->GetPawn());
    if (!Character) return;

    const auto PlayerState = Cast<AAPlayerState>(Controller->PlayerState);
    if (!PlayerState) return;

    Character->SetPlayerColor(PlayerState->GetTeamColor());
}

void AAloneGameModeBase::Killed(AController* KillerController, AController* VictimController)
{
    const auto KillerPlayerState = KillerController ? Cast<AAPlayerState>(KillerController->PlayerState) : nullptr;
    const auto VictimPlayerState = VictimController ? Cast<AAPlayerState>(VictimController->PlayerState) : nullptr;

    if (KillerPlayerState)
    {
        KillerPlayerState->AddKill();
    }

    if (VictimPlayerState)
    {
        VictimPlayerState->AddDeath();
    }

    StartRespawn(VictimController);
}

void AAloneGameModeBase::LogPlayerInfo()
{
    if (!GetWorld()) return;

    int32 TeamID = 1;
    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto Controller = It->Get();
        if (!Controller) continue;

        const auto PlayerState = Cast<AAPlayerState>(Controller->PlayerState);
        if (!PlayerState) continue;

        PlayerState->LogInfo();
    }
}

void AAloneGameModeBase::StartRespawn(AController* Controller)
{
    const auto RespawnAvailable = RoundCountDown > MinRoundTimeForRespawn + GameData.RespawnTime;
    if (!RespawnAvailable) return;

    const auto RespawnComponent = AUtils::GetAPlayerComponent<UARespawnComponent>(Controller);
    if (!RespawnComponent) return;

    RespawnComponent->Respawn(GameData.RespawnTime);
}

void AAloneGameModeBase::RespawnRequest(AController* Controller)
{
    ResetOnePlayer(Controller);
}

void AAloneGameModeBase::GameOver() 
{
    UE_LOG(LogAGameModeBase, Display, TEXT("Game Over!"));
    LogPlayerInfo();

    for (auto Pawn : TActorRange<APawn>(GetWorld()))
    {
        if (Pawn)
        {
            Pawn->TurnOff();
            Pawn->DisableInput(nullptr);
        }
    }

    SetMatchState(EAMatchState::GameOver);
}

void AAloneGameModeBase::SetMatchState(EAMatchState State)
{
    if (MatchState == State) return;

    MatchState = State;
    OnMatchStateChanged.Broadcast(MatchState);
}
