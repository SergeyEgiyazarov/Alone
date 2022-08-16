// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/AWeaponComponent.h"
#include "Weapon/ABaseWeapon.h"
#include "GameFramework/Character.h"
#include "Animations/AEquipFinichAnimNotify.h"
#include "Animations/AReloadFinishedAnimNotify.h"
#include "Animations/AEquipWeaponAnimNotify.h"
#include "Animations/AnimUtils.h"

DEFINE_LOG_CATEGORY_STATIC(WeaponComponentLog, All, All);

UAWeaponComponent::UAWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UAWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    CurrentWeaponIndex = 0;
    InitAnimations();
    SpawnWeapons();
    EquipWeapon(CurrentWeaponIndex);
}

void UAWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    CurrentWeapon = nullptr;

    for (auto Weapon : Weapons)
    {
        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        Weapon->Destroy();
    }
    Weapons.Empty();

    Super::EndPlay(EndPlayReason);
}

void UAWeaponComponent::SpawnWeapons()
{
    const auto Character = Cast<ACharacter>(GetOwner());
    if (!Character || !GetWorld()) return;

    for (auto OneWeaponData : WeaponData)
    {
        auto Weapon = GetWorld()->SpawnActor<AABaseWeapon>(OneWeaponData.WeaponClass);
        if (!Weapon) continue;

        Weapon->SetOwner(Character);
        Weapon->OnClipEmpty.AddUObject(this, &UAWeaponComponent::OnClipEmpty);
        Weapons.Add(Weapon);

        AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
    }
}

void UAWeaponComponent::AttachWeaponToSocket(AABaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)
{
    if (!Weapon || !SceneComponent) return;

    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void UAWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
    if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num()) return;

    const auto Character = Cast<ACharacter>(GetOwner());
    if (!Character) return;

    if (CurrentWeapon)
    {
        //CurrentWeapon->StopFire();
        AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
    }

    CurrentWeapon = Weapons[WeaponIndex];

    auto CurrentWeaponDate = WeaponData.FindByPredicate(
        [&](const FWeaponData& Data)
        {
            return Data.WeaponClass == CurrentWeapon->GetClass();
        });
    CurrentReloadAnimMontage = CurrentWeaponDate ? CurrentWeaponDate->RealoadAnimMontage : nullptr;

    AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
}

void UAWeaponComponent::StartFire()
{
    if (!CanFire()) return;
    CurrentWeapon->StartFire();
}

void UAWeaponComponent::StopFire()
{
    if (!CurrentWeapon) return;
    CurrentWeapon->StopFire();
}

void UAWeaponComponent::NextWeapon()
{
    if (!CanEquip()) return;

    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    StartEquipWeapon();
}

void UAWeaponComponent::StartEquipWeapon()
{
    EquipAnimInProgress = true;
    CurrentWeapon->StopFire();
    PlayAnimMontage(EquipAnimMontage);
}

void UAWeaponComponent::Reload()
{
    ChangeClip();
}

void UAWeaponComponent::PlayAnimMontage(UAnimMontage* Animation)
{
    const auto Character = Cast<ACharacter>(GetOwner());
    if (!Character) return;

    Character->PlayAnimMontage(Animation);
}

void UAWeaponComponent::InitAnimations()
{
    auto EquipWeaponNotify = AnimUtils::FindNotifyByClass<UAEquipWeaponAnimNotify>(EquipAnimMontage);
    if (EquipWeaponNotify)
    {
        EquipWeaponNotify->OnNotified.AddUObject(this, &UAWeaponComponent::OnChangeWeapon);
    }
    else
    {
        UE_LOG(WeaponComponentLog, Error, TEXT("Equip weapon anim notify is empty"));
        checkNoEntry();
    }

    auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<UAEquipFinichAnimNotify>(EquipAnimMontage);
    if (EquipFinishedNotify)
    {
        EquipFinishedNotify->OnNotified.AddUObject(this, &UAWeaponComponent::OnEquipFinished);
    }
    else
    {
        UE_LOG(WeaponComponentLog, Error, TEXT("Equip anim notify is empty"));
        checkNoEntry();
    }

    for (auto OneWeaponData : WeaponData)
    {
        auto ReloadFinishedNotify = AnimUtils::FindNotifyByClass<UAReloadFinishedAnimNotify>(OneWeaponData.RealoadAnimMontage);
        if (!ReloadFinishedNotify)
        {
            UE_LOG(WeaponComponentLog, Error, TEXT("Reload anim notify is empty"));
            checkNoEntry();
        }

        ReloadFinishedNotify->OnNotified.AddUObject(this, &UAWeaponComponent::OnReloadFinished);
    }
}

void UAWeaponComponent::OnChangeWeapon(USkeletalMeshComponent* MeshComp)
{
    const auto Character = Cast<ACharacter>(GetOwner());
    if (!Character || Character->GetMesh() != MeshComp) return;

    EquipWeapon(CurrentWeaponIndex);
}

void UAWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComp)
{
    const auto Character = Cast<ACharacter>(GetOwner());
    if (!Character || Character->GetMesh() != MeshComp) return;

    EquipAnimInProgress = false;
}

void UAWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComp)
{
    const auto Character = Cast<ACharacter>(GetOwner());
    if (!Character || Character->GetMesh() != MeshComp) return;

    ReloadAnimInProgress = false;
}

bool UAWeaponComponent::CanFire() const
{
    return (CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress);
}

bool UAWeaponComponent::CanEquip() const
{
    return !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool UAWeaponComponent::CanReload() const
{
    return (CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress && CurrentWeapon->CanReload());
}

void UAWeaponComponent::OnClipEmpty()
{
    ChangeClip();
}

void UAWeaponComponent::ChangeClip()
{
    if (!CanReload()) return;
    CurrentWeapon->StopFire();
    CurrentWeapon->ChangeClips();
    ReloadAnimInProgress = true;
    PlayAnimMontage(CurrentReloadAnimMontage);
}

bool UAWeaponComponent::GetWeaponUIData(FWeaponUIData& UIData) const
{
    if (CurrentWeapon)
    {
        UIData = CurrentWeapon->GetUIData();
        return true;
    }
    return false;
}

bool UAWeaponComponent::GetWeaponAmmoData(FAmmoData& AmmoData) const
{
    if (CurrentWeapon)
    {
        AmmoData = CurrentWeapon->GetAmmoData();
        return true;
    }
    return false;
}

bool UAWeaponComponent::TryToAddAmmo(TSubclassOf<AABaseWeapon> WeaponType, int32 AmmoAmount)
{
    for (const auto Weapon : Weapons)
    {
        if (Weapon && Weapon->IsA(WeaponType))
        {
            return Weapon->TryToAddAmmo(AmmoAmount);
        }
    }
    return false;
}
