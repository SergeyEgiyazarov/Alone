// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ABaseWidget.h"
#include "APauseWidget.generated.h"

class UButton;

UCLASS()
class ALONE_API UAPauseWidget : public UABaseWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeOnInitialized() override;

    UPROPERTY(meta = (BindWidget))
    UButton* ClearPauseButton;

private:
    UFUNCTION()
    void OnClearPuse();
};
