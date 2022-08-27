// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Decorators/AHealthPercentDecorator.h"
#include "AIController.h"
#include "AUtils.h"
#include "Components/AHealthComponent.h"

UAHealthPercentDecorator::UAHealthPercentDecorator()
{
    NodeName = "Health Percent";
}

bool UAHealthPercentDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller) return false;

    const auto HealthComponent = AUtils::GetAPlayerComponent<UAHealthComponent>(Controller->GetPawn());
    if (!HealthComponent || HealthComponent->IsDead()) return false;

    return HealthComponent->GetHealthPercent() <= HealthPercent && Probability > 0 && FMath::FRand() <= Probability;
}
