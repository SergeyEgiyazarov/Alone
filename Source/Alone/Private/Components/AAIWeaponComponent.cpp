// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/AAIWeaponComponent.h"
#include "Weapon/ABaseWeapon.h"

void UAAIWeaponComponent::StartFire()
{
    if (!CanFire()) return;

    if (CurrentWeapon->IsAmmoEmpty())
    {
        NextWeapon();
    }
    else
    {
        CurrentWeapon->StartFire();
    }
}

void UAAIWeaponComponent::NextWeapon()
{
    if (!CanEquip()) return;

    int32 NextIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    while (NextIndex != CurrentWeaponIndex)
    {
        if (!Weapons[NextIndex]->IsAmmoEmpty()) break;
        NextIndex = (NextIndex + 1) % Weapons.Num();
    }

    if (NextIndex != CurrentWeaponIndex)
    {
        CurrentWeaponIndex = NextIndex;
        StartEquipWeapon();
    }
}
