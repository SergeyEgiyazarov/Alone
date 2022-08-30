// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/ABaseWeapon.h"
#include "ARifleWeapon.generated.h"

class UAWeaponFXComponent;
class UNiagaraSystem;

UCLASS()
class ALONE_API AARifleWeapon : public AABaseWeapon
{
    GENERATED_BODY()

public:
    AARifleWeapon();

    virtual void StartFire() override;
    virtual void StopFire() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float TimeBetweenShots = 0.1f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float BulletSpread = 1.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float DamageAmount = 10.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem* TraceFX;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FString TraceTargetName = "TraceTarget";

    UPROPERTY(VisibleAnywhere, Category = "VFX")
    UAWeaponFXComponent* WeaponFXComponent;

    virtual void BeginPlay() override;

    virtual void MakeShot() override;
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;
    void MakeDamage(const FHitResult& HitResult);

private:
    FTimerHandle ShotTimerHandle;

    void SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd);
    AController* GetController() const;
};
