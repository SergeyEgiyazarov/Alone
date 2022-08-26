// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AWeaponComponent.h"
#include "AAIWeaponComponent.generated.h"

UCLASS()
class ALONE_API UAAIWeaponComponent : public UAWeaponComponent
{
    GENERATED_BODY()

public:
    virtual void StartFire() override;
    virtual void NextWeapon() override;
};
