// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AAIController.generated.h"

class UAAIPerceptionComponent;

UCLASS()
class ALONE_API AAAIController : public AAIController
{
    GENERATED_BODY()

public:
    AAAIController();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
    UAAIPerceptionComponent* AAIPerceptionComponent;

    virtual void OnPossess(APawn* InPawn) override;
    virtual void Tick(float DeltaTime) override;
};
