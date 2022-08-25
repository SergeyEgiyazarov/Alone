// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Services/AFindEnemyService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/AAIPerceptionComponent.h"
#include "AIController.h"
#include "AUtils.h"

UAFindEnemyService::UAFindEnemyService()
{
    NodeName = "Find Enemy";
}

void UAFindEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (Blackboard)
    {
        const auto Controller = OwnerComp.GetAIOwner();
        const auto PerceptionComponent = AUtils::GetAPlayerComponent<UAAIPerceptionComponent>(Controller);
        if (PerceptionComponent)
        {
            Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, PerceptionComponent->GetClosestEnemy());
        }
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
