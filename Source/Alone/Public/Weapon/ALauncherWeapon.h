// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/ABaseWeapon.h"
#include "ALauncherWeapon.generated.h"

class AAProjectile;

UCLASS()
class ALONE_API AALauncherWeapon : public AABaseWeapon
{
    GENERATED_BODY()

public:
    virtual void StartFire() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<AAProjectile> ProjectileClass;

    virtual void MakeShot() override;
};
