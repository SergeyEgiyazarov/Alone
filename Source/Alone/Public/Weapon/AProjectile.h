// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UAWeaponFXComponent;

UCLASS()
class ALONE_API AAProjectile : public AActor
{
    GENERATED_BODY()

public:
    AAProjectile();

    void SetShotDirection(const FVector& Direction);

protected:
    UPROPERTY(VisibleAnywhere, Category = "Weapon")
    USphereComponent* CollisionComponent;

    UPROPERTY(VisibleAnywhere, Category = "Weapon")
    UProjectileMovementComponent* MovementComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float DamageRadius = 200.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float DamageAmount = 50.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    bool DoFullDamage = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float LifeSeconds = 5.0f;

    UPROPERTY(VisibleAnywhere, Category = "VFX")
    UAWeaponFXComponent* WeaponFXComponent;

    virtual void BeginPlay() override;

private:
    FVector ShotDirection;

    UFUNCTION()
    void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
        const FHitResult& Hit);

    AController* GetController() const;
};
