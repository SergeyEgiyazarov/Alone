// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AGoToMenuWidget.generated.h"

class UButton;

UCLASS()
class ALONE_API UAGoToMenuWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* MainMenuButton;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void GoToMainMenu();
};
