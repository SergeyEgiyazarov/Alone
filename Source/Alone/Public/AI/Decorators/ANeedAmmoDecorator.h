// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "ANeedAmmoDecorator.generated.h"

class AABaseWeapon;

UCLASS()
class ALONE_API UANeedAmmoDecorator : public UBTDecorator
{
    GENERATED_BODY()

public:
    UANeedAmmoDecorator();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (ClampMin = "0.01", ClampMax = "1.0"))
    float Probability = 0.4f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    TSubclassOf<AABaseWeapon> WeaponType;

    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
