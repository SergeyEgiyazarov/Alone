// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ABasePickup.generated.h"

class USphereComponent;

UCLASS()
class ALONE_API AABasePickup : public AActor
{
    GENERATED_BODY()

public:
    AABasePickup();

protected:
    UPROPERTY(VisibleAnywhere, Category = "Pickup")
    USphereComponent* CollisionComponent;

    virtual void BeginPlay() override;
    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:
    virtual void Tick(float DeltaTime) override;

private:
    float RotationYaw = 1.0f;

    virtual bool GivePickupTo(APawn* PlayerPawn);
};
