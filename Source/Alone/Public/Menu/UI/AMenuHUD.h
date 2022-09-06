// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AMenuHUD.generated.h"

UCLASS()
class ALONE_API AAMenuHUD : public AHUD
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> MenuWidgetClass;

    virtual void BeginPlay() override;
};
