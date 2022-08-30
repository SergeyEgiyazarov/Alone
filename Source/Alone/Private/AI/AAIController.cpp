// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/AAIController.h"
#include "AI/AAICharacter.h"
#include "Components/AAIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

AAAIController::AAAIController()
{
    AAIPerceptionComponent = CreateDefaultSubobject<UAAIPerceptionComponent>("AIPerceptionComponent");
    SetPerceptionComponent(*AAIPerceptionComponent);

    bWantsPlayerState = true;
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

    const auto AimActor = GetFocuseOnActor();
    SetFocus(AimActor);
}

AActor* AAAIController::GetFocuseOnActor() const
{
    if (!GetBlackboardComponent()) return nullptr;
    return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocuseOnKeyName));
}
