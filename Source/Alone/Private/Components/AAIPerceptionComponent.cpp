// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/AAIPerceptionComponent.h"
#include "AIController.h"
#include "AUtils.h"
#include "Components/AHealthComponent.h"
#include "Perception/AISense_Sight.h"

AActor* UAAIPerceptionComponent::GetClosestEnemy() const
{
    TArray<AActor*> PerceiveActors;
    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceiveActors);
    if (PerceiveActors.Num() == 0) return nullptr;

    const auto Controller = Cast<AAIController>(GetOwner());
    if (!Controller) return nullptr;

    const auto Pawn = Controller->GetPawn();
    if (!Pawn) return nullptr;

    float BestDistance = MAX_FLT;
    AActor* BestActor = nullptr;

    for (const auto PerceiveActor : PerceiveActors)
    {
        const auto HealthComponent = AUtils::GetAPlayerComponent<UAHealthComponent>(PerceiveActor);
        if (HealthComponent && !HealthComponent->IsDead())
        {
            const auto CurrentDistance = (PerceiveActor->GetActorLocation() - Pawn->GetActorLocation()).Size();
            if (CurrentDistance < BestDistance)
            {
                BestDistance = CurrentDistance;
                BestActor = PerceiveActor;
            }
        }
    }

    return BestActor;
}
