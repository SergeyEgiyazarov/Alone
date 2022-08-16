// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/ABasePickup.h"
#include "AAmmoPickup.generated.h"

class AABaseWeapon;

UCLASS()
class ALONE_API AAAmmoPickup : public AABasePickup
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickUp", meta = (ClampMin = "10.0", ClampMax = "350.0"))
    int32 AmmoAmount = 150;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickUp")
    TSubclassOf<AABaseWeapon> WeaponType;

private:
    virtual bool GivePickupTo(APawn* PlayerPawn) override;
};
