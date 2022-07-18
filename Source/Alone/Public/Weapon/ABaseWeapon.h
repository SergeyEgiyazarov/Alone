// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ABaseWeapon.generated.h"

class USkeletalMeshComponent;

UCLASS()
class ALONE_API AABaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    AABaseWeapon();

    virtual void StartFire();
    virtual void StopFire();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FName MuzzleSocketName = "MuzzleFlash";

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    float TraceMaxDistance = 1500.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float DamageAmount = 10.0f;

    virtual void BeginPlay() override;

    virtual void MakeShot();
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;

    APlayerController* GetPlayerController() const;
    bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
    FVector GetMuzzleWorldLocation() const;
    void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);
    void MakeDamage(const FHitResult& HitResult);
};
