// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ASpectatorWidget.generated.h"

UCLASS()
class ALONE_API UASpectatorWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    bool GetRespawnTime(int32& RespawnCountDown) const;
};
