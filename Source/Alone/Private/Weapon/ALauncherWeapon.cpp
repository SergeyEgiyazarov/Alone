// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/ALauncherWeapon.h"
#include "Weapon/AProjectile.h"
#include "Kismet/GameplayStatics.h"

void AALauncherWeapon::StartFire()
{
    MakeShot();
}

void AALauncherWeapon::MakeShot()
{
    if (!GetWorld() || IsAmmoEmpty()) return;

    if (IsClipsEmpty())
    {
        OnClipEmpty.Broadcast(this);
        return;
    }

    FVector TraceStart;
    FVector TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd)) return;

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
    FVector Direction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal();

    const FTransform SpawnTransform(WeaponMesh->GetSocketRotation(MuzzleSocketName), GetMuzzleWorldLocation());
    AAProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAProjectile>(ProjectileClass, SpawnTransform);

    if (Projectile)
    {
        Projectile->SetShotDirection(Direction);
        Projectile->SetOwner(GetOwner());
        Projectile->FinishSpawning(SpawnTransform);
    }

    DecreaseAmmo();
    SpawnMuzzleFX();
}
