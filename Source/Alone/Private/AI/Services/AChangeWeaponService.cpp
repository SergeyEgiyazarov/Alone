// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Services/AChangeWeaponService.h"
#include "Components/AWeaponComponent.h"
#include "AIController.h"
#include "AUtils.h"

UAChangeWeaponService::UAChangeWeaponService()
{
    NodeName = "Change Weapon";
}

void UAChangeWeaponService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const auto Controller = OwnerComp.GetAIOwner();

    if (Controller && Probability > 0.0f && FMath::FRand() <= Probability)
    {
        const auto WeaponComponent = AUtils::GetAPlayerComponent<UAWeaponComponent>(Controller->GetPawn());
        if (WeaponComponent)
        {
            WeaponComponent->NextWeapon();
        }
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
