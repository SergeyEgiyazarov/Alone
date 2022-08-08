// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/APlayerHUDWidget.h"
#include "Components/AHealthComponent.h"
#include "Components/AWeaponComponent.h"
#include "AUtils.h"

float UAPlayerHUDWidget::GetHealthPercent() const
{
    const auto HealthComponent = AUtils::GetAPlayerComponent<UAHealthComponent>(GetOwningPlayerPawn());
    if (!HealthComponent) return 0.0f;

    return HealthComponent->GetHealthPercent();
}

bool UAPlayerHUDWidget::GetWeaponUIData(FWeaponUIData& UIData) const
{
    const auto WeaponComponent = AUtils::GetAPlayerComponent<UAWeaponComponent>(GetOwningPlayerPawn());
    if (!WeaponComponent) return false;

    return WeaponComponent->GetWeaponUIData(UIData);
}

bool UAPlayerHUDWidget::GetWeaponAmmoData(FAmmoData& AmmoData) const
{
    const auto WeaponComponent = AUtils::GetAPlayerComponent<UAWeaponComponent>(GetOwningPlayerPawn());
    if (!WeaponComponent) return false;

    return WeaponComponent->GetWeaponAmmoData(AmmoData);
}

bool UAPlayerHUDWidget::IsPlayerAlive() const
{
    const auto HealthComponent = AUtils::GetAPlayerComponent<UAHealthComponent>(GetOwningPlayerPawn());
    return HealthComponent && !HealthComponent->IsDead();
}

bool UAPlayerHUDWidget::IsPlayerSpectating() const
{
    const auto Controller = GetOwningPlayer();
    return Controller && Controller->GetStateName() == NAME_Spectating;
}