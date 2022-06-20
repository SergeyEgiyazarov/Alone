// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/ABaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"

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
}

void AABaseWeapon::Fire()
{
    UE_LOG(BaseWeaponLog, Display, TEXT("Fire!"));
}
