// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "AEnemyEnvQueryContext.generated.h"

UCLASS()
class ALONE_API UAEnemyEnvQueryContext : public UEnvQueryContext
{
    GENERATED_BODY()

public:
    virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FName EnemyActorKeyName = "EnemyActor";
};
