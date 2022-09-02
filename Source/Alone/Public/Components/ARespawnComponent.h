// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ARespawnComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ALONE_API UARespawnComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UARespawnComponent();

    void Respawn(int32 RespawnTime);
    int32 GetRespawnCountDown() const { return RespawnCountDown; }

    bool IsRespawnIsProgress() const;

private:
    FTimerHandle RespawnTimerHandle;
    int32 RespawnCountDown;

    void RespawnTimerUpdate();
};
