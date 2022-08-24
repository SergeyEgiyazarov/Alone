// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/AAIController.h"
#include "AI/AAICharacter.h"
#include "Components/AAIPerceptionComponent.h"

AAAIController::AAAIController()
{
    AAIPerceptionComponent = CreateDefaultSubobject<UAAIPerceptionComponent>("AIPerceptionComponent");
    SetPerceptionComponent(*AAIPerceptionComponent);
}

void AAAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    const auto AICharacter = Cast<AAAICharacter>(InPawn);
    if (AICharacter)
    {
        RunBehaviorTree(AICharacter->BehaviorTreeAsset);
    }
}

void AAAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    const auto AimActor = AAIPerceptionComponent->GetClosestEnemy();
    SetFocus(AimActor);
}
