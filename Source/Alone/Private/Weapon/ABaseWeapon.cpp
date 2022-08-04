// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/ABaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"

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
    checkf(DefaultAmmo.Clips > 0, TEXT("Clips count couldn't be less or equal zero"));

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
    const auto Controller = GetPlayerController();
    if (!Controller) return false;

    Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
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

    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
}

void AABaseWeapon::DecreaseAmmo()
{

    if (CurrentAmmo.Bullets == 0)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "--- No Bullets ---");
        return;
    }

    --CurrentAmmo.Bullets;
    LogAmmo();

    if (IsClipsEmpty() && !IsAmmoEmpty())
    {
        StopFire();
        OnClipEmpty.Broadcast();
    }
}

bool AABaseWeapon::IsAmmoEmpty() const
{
    if (CurrentAmmo.Clips == 0)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "--- No Clips ---");
    }
    return CurrentAmmo.Clips == 0 && !CurrentAmmo.Infinite && IsClipsEmpty();
}

bool AABaseWeapon::IsClipsEmpty() const
{
    return CurrentAmmo.Bullets == 0;
}

void AABaseWeapon::ChangeClips()
{
    if (!CurrentAmmo.Infinite)
    {
        --CurrentAmmo.Clips;
    }
    CurrentAmmo.Bullets = DefaultAmmo.Bullets;
    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "--- Change Clips ---");
}

bool AABaseWeapon::CanReload() const
{
    return CurrentAmmo.Bullets < DefaultAmmo.Bullets && CurrentAmmo.Clips > 0;
}

void AABaseWeapon::LogAmmo()
{
    FString Info = "Ammo Info: " + FString::FromInt(CurrentAmmo.Bullets) + "/";
    Info += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);

    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, Info);
}
