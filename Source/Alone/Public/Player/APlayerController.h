// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "APlayerController.generated.h"

class UARespawnComponent;

UCLASS()
class ALONE_API AAPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AAPlayerController();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UARespawnComponent* RespawnComponent;
};
