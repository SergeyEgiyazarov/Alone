// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/ABasePickup.h"
#include "AHealthPickup.generated.h"

UCLASS()
class ALONE_API AAHealthPickup : public AABasePickup
{
    GENERATED_BODY()

private:
    virtual bool GivePickupTo(APawn* PlayerPawn) override;
};
