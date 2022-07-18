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
    const FTransform SpawnTransform(WeaponMesh->GetSocketRotation(MuzzleSocketName), GetMuzzleWorldLocation());
    auto Projectile = UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), ProjectileClass, SpawnTransform);

    UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);
}
