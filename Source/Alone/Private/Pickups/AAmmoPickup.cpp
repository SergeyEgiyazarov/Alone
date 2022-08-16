// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickups/AAmmoPickup.h"
#include "Components/AHealthComponent.h"
#include "Components/AWeaponComponent.h"
#include "AUtils.h"

DEFINE_LOG_CATEGORY_STATIC(AmmoPickupLog, All, All);

bool AAAmmoPickup::GivePickupTo(APawn* PlayerPawn)
{
    const auto HealthComponent = AUtils::GetAPlayerComponent<UAHealthComponent>(PlayerPawn);
    if (!HealthComponent || HealthComponent->IsDead()) return false;

    const auto WeaponComponent = AUtils::GetAPlayerComponent<UAWeaponComponent>(PlayerPawn);
    if (!WeaponComponent) return false;

    UE_LOG(AmmoPickupLog, Display, TEXT("Ammo pickup was taken"));
    return WeaponComponent->TryToAddAmmo(WeaponType, AmmoAmount);
}
