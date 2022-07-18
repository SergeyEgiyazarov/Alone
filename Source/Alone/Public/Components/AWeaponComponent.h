// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AWeaponComponent.generated.h"

class AABaseWeapon;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ALONE_API UAWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UAWeaponComponent();

    void StartFire();
    void StopFire();

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<AABaseWeapon> WeaponClass;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponAttachPointName = "WeaponSocket";

    virtual void BeginPlay() override;

private:
    UPROPERTY()
    AABaseWeapon* CurrentWeapon = nullptr;

    void SpawnWeapon();
};
