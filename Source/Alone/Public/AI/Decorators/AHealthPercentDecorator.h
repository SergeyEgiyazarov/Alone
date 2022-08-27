// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "AHealthPercentDecorator.generated.h"

UCLASS()
class ALONE_API UAHealthPercentDecorator : public UBTDecorator
{
    GENERATED_BODY()

public:
    UAHealthPercentDecorator();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (ClampMin = "0.01", ClampMax = "1.0"))
    float Probability = 0.7f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float HealthPercent = 0.4f;

    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
