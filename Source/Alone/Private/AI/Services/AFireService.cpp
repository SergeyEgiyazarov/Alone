// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Services/AFireService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/AWeaponComponent.h"
#include "AIController.h"
#include "AUtils.h"

UAFireService::UAFireService()
{
    NodeName = "Fire";
}

void UAFireService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const auto Controller = OwnerComp.GetAIOwner();
    const auto Blackboard = Controller->GetBlackboardComponent();

    const auto HisAim = Blackboard && Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName);

    if (Controller)
    {
        const auto WeaponComponent = AUtils::GetAPlayerComponent<UAWeaponComponent>(Controller->GetPawn());
        if (WeaponComponent)
        {
            HisAim ? WeaponComponent->StartFire() : WeaponComponent->StopFire();
        }
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
