// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/APlayerController.h"
#include "Components/ARespawnComponent.h"
#include "Gameframework/GameModeBase.h"
#include "Alone/AloneGameModeBase.h"

AAPlayerController::AAPlayerController()
{
    RespawnComponent = CreateDefaultSubobject<UARespawnComponent>("RespawnComponent");
}

void AAPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (GetWorld())
    {
        const auto GameMode = Cast<AAloneGameModeBase>(GetWorld()->GetAuthGameMode());
        if (GameMode)
        {
            GameMode->OnMatchStateChanged.AddUObject(this, &AAPlayerController::OnMatchStateChanged);
        }
    }
}

void AAPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    OnNewPawn.Broadcast(InPawn);
}

void AAPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    if (!InputComponent) return;

    InputComponent->BindAction("PauseGame", IE_Pressed, this, &AAPlayerController::OnPauseGame);
}

void AAPlayerController::OnPauseGame()
{
    if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

    GetWorld()->GetAuthGameMode()->SetPause(this);
}

void AAPlayerController::OnMatchStateChanged(EAMatchState State)
{
    if (State == EAMatchState::InProgress)
    {
        SetInputMode(FInputModeGameOnly());
        bShowMouseCursor = false;
    }
    else
    {
        SetInputMode(FInputModeUIOnly());
        bShowMouseCursor = true;
    }
}
