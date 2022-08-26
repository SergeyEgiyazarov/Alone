// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/ABaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "NiagaraFunctionLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(BaseWeaponLog, All, All);

AABaseWeapon::AABaseWeapon()
{
    PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh);
}

void AABaseWeapon::BeginPlay()
{
    Super::BeginPlay();

    check(WeaponMesh);
    checkf(DefaultAmmo.Bullets > 0, TEXT("Bullets count couldn't be less or equal zero"));
    checkf(DefaultAmmo.BulletsInInventory > 0, TEXT("Clips count couldn't be less or equal zero"));

    CurrentAmmo = DefaultAmmo;
}

void AABaseWeapon::StartFire()
{
}

void AABaseWeapon::StopFire()
{
}

void AABaseWeapon::MakeShot()
{
}

APlayerController* AABaseWeapon::GetPlayerController() const
{
    const auto Player = Cast<ACharacter>(GetOwner());
    if (!Player) return nullptr;

    return Player->GetController<APlayerController>();
}

bool AABaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
    const auto AACharacter = Cast<ACharacter>(GetOwner());
    if (!AACharacter) return false;

    if (AACharacter->IsPlayerControlled())
    {
        const auto Controller = GetPlayerController();
        if (!Controller) return false;

        Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
    }
    else
    {
        ViewLocation = GetMuzzleWorldLocation();
        ViewRotation = WeaponMesh->GetSocketRotation(MuzzleSocketName);
    }

    return true;
}

FVector AABaseWeapon::GetMuzzleWorldLocation() const
{
    return WeaponMesh->GetSocketTransform(MuzzleSocketName).GetLocation();
}

bool AABaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

    TraceStart = ViewLocation;
    const FVector ShootDirection = ViewRotation.Vector();
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
    return true;
}

void AABaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
    if (!GetWorld()) return;

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
    CollisionParams.bReturnPhysicalMaterial = true;

    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
}

void AABaseWeapon::DecreaseAmmo()
{

    if (CurrentAmmo.Bullets == 0) return;

    --CurrentAmmo.Bullets;

    if (IsClipsEmpty() && !IsAmmoEmpty())
    {
        StopFire();
        OnClipEmpty.Broadcast(this);
    }
}

bool AABaseWeapon::IsAmmoEmpty() const
{
    return CurrentAmmo.BulletsInInventory == 0 && !CurrentAmmo.Infinite && IsClipsEmpty();
}

bool AABaseWeapon::IsClipsEmpty() const
{
    return CurrentAmmo.Bullets == 0;
}

void AABaseWeapon::ChangeClips()
{
    if (!CurrentAmmo.Infinite)
    {
        //--CurrentAmmo.BulletsInInventory;
        if (CurrentAmmo.BulletsInInventory < DefaultAmmo.Bullets)
        {
            CurrentAmmo.Bullets = CurrentAmmo.BulletsInInventory;
            CurrentAmmo.BulletsInInventory = 0;
        }
        else
        {
            int32 RequiredBullets = DefaultAmmo.Bullets - CurrentAmmo.Bullets;
            CurrentAmmo.Bullets = DefaultAmmo.Bullets;
            CurrentAmmo.BulletsInInventory -= RequiredBullets;
        }
    }
    else
    {
        CurrentAmmo.Bullets = DefaultAmmo.Bullets;
    }
}

bool AABaseWeapon::CanReload() const
{
    return CurrentAmmo.Bullets < DefaultAmmo.Bullets && CurrentAmmo.BulletsInInventory > 0;
}

void AABaseWeapon::LogAmmo()
{
    FString Info = "Ammo Info: " + FString::FromInt(CurrentAmmo.Bullets) + "/";
    Info += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.BulletsInInventory);

    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, Info);
}

bool AABaseWeapon::IsAmmoFull() const
{
    return CurrentAmmo.BulletsInInventory == DefaultAmmo.BulletsInInventory && //
           CurrentAmmo.Bullets == DefaultAmmo.Bullets;
}

bool AABaseWeapon::TryToAddAmmo(int32 AmmoAmount)
{
    if (CurrentAmmo.Infinite || IsAmmoFull() || AmmoAmount <= 0) return false;

    if (IsAmmoEmpty())
    {
        CurrentAmmo.BulletsInInventory = FMath::Clamp(AmmoAmount, 0, DefaultAmmo.BulletsInInventory);
        OnClipEmpty.Broadcast(this);
    }
    else if (CurrentAmmo.BulletsInInventory < DefaultAmmo.BulletsInInventory)
    {
        const auto NextAmmoAmount = CurrentAmmo.BulletsInInventory + AmmoAmount;
        if (DefaultAmmo.BulletsInInventory - NextAmmoAmount >= 0)
        {
            CurrentAmmo.BulletsInInventory = NextAmmoAmount;
        }
        else
        {
            CurrentAmmo.BulletsInInventory = DefaultAmmo.BulletsInInventory;
        }
    }
    else
    {
        return false;
    }

    return true;
}

void AABaseWeapon::SpawnMuzzleFX()
{
    UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleFX, //
        WeaponMesh,                                        //
        MuzzleSocketName,                                  //
        FVector::ZeroVector,                               //
        FRotator::ZeroRotator,                             //
        EAttachLocation::SnapToTarget, true);
}
