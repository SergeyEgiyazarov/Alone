// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Decorators/ANeedAmmoDecorator.h"
#include "AIController.h"
#include "AUtils.h"
#include "Components/AWeaponComponent.h"
#include "ABaseWeapon.h"

UANeedAmmoDecorator::UANeedAmmoDecorator()
{
    NodeName = "Need Ammo";
}

bool UANeedAmmoDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller) return false;

    const auto WeaponComponent = AUtils::GetAPlayerComponent<UAWeaponComponent>(Controller->GetPawn());
    if (!WeaponComponent) return false;

    return WeaponComponent->NeedAmmo(WeaponType) && Probability > 0 && FMath::FRand() <= Probability;
}
