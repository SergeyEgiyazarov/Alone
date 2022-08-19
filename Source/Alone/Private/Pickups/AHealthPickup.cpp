// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickups/AHealthPickup.h"
#include "Components/AHealthComponent.h"
#include "AUtils.h"

DEFINE_LOG_CATEGORY_STATIC(HealthPickupLog, All, All);

bool AAHealthPickup::GivePickupTo(APawn* PlayerPawn)
{
    const auto HealthComponent = AUtils::GetAPlayerComponent<UAHealthComponent>(PlayerPawn);
    if (!HealthComponent) return false;

    UE_LOG(HealthPickupLog, Display, TEXT("Health pickup was taken"));
    return HealthComponent->TryToAddHealth(HealthAmount);
}