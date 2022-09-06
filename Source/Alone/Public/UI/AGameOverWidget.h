// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ABaseWidget.h"
#include "ACoreTypes.h"
#include "AGameOverWidget.generated.h"

class UVerticalBox;
class UButton;

UCLASS()
class ALONE_API UAGameOverWidget : public UABaseWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UVerticalBox* PlayerStatBox;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> PlayerStatRowWidgetClass;

    UPROPERTY(meta = (BindWidget))
    UButton* ResetLevelButton;

    virtual void NativeOnInitialized() override;

private:
    void OnMatchStateChanged(EAMatchState State);
    void UpdatePlayerStat();

    UFUNCTION()
    void OnResetLevel();
};
