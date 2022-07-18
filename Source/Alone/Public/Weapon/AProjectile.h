// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AProjectile.generated.h"

class USphereComponent;
UCLASS()
class ALONE_API AAProjectile : public AActor
{
    GENERATED_BODY()

public:
    AAProjectile();

protected:

    UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
    USphereComponent* CollisionComponent;

    virtual void BeginPlay() override;

};
