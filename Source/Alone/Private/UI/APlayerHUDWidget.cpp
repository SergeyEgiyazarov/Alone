// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/APlayerHUDWidget.h"
#include "Components/AHealthComponent.h"
#include "Components/AWeaponComponent.h"
#include "AUtils.h"
#include "Components/ProgressBar.h"

void UAPlayerHUDWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (GetOwningPlayer())
    {
        GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &UAPlayerHUDWidget::OnNewPawn);
        OnNewPawn(GetOwningPlayerPawn());
    }
}

void UAPlayerHUDWidget::OnNewPawn(APawn* NewPawn)
{
    const auto HealthComponent = AUtils::GetAPlayerComponent<UAHealthComponent>(GetOwningPlayerPawn());
    if (HealthComponent && !HealthComponent->OnHealthChanged.IsBoundToObject(this))
    {
        HealthComponent->OnHealthChanged.AddUObject(this, &UAPlayerHUDWidget::OnHealthChange);
    }

    UpdateHealthColor();
}

void UAPlayerHUDWidget::OnHealthChange(float Health, float HealthDelta)
{
    if (HealthDelta < 0.0f)
    {
        OnTakeDamage();

        if (!IsAnimationPlaying(DamageAnimation))
        {
            PlayAnimation(DamageAnimation);
        }
    }

    UpdateHealthColor();
}

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


void UAPlayerHUDWidget::UpdateHealthColor()
{
    if (HealthProgressBar)
    {
        HealthProgressBar->SetFillColorAndOpacity(GetHealthPercent() > PercentColorThreshold ? GoodColor : BadColor);
    }
}

FString UAPlayerHUDWidget::FormatBullets(int32 BulletsNum) const
{
    const int32 MaxLen = 3;
    const TCHAR PrefixSymbol = '0';

    auto BulletStr = FString::FromInt(BulletsNum);
    const auto SymbolNumToAdd = MaxLen - BulletStr.Len();

    if (SymbolNumToAdd > 0)
    {
        BulletStr = FString::ChrN(SymbolNumToAdd, PrefixSymbol).Append(BulletStr);
    }

    return BulletStr;
}
