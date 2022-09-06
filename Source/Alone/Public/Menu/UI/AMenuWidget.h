// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ACoreTypes.h"
#include "AMenuWidget.generated.h"

class UButton;
class UHorizontalBox;
class UAGameInstance;
class UALevelItemWidget;

UCLASS()
class ALONE_API UAMenuWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* StartGameButton;

    UPROPERTY(meta = (BindWidget))
    UButton* QuitGameButton;

    UPROPERTY(meta = (BindWidget))
    UHorizontalBox* LevelItemsBox;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> LevelItemWidgetClass;

    virtual void NativeOnInitialized() override;

private:
    UPROPERTY()
    TArray<UALevelItemWidget*> LevelItemWidgets;

    UFUNCTION()
    void OnStartGame();

    UFUNCTION()
    void OnQuitGame();

    void InitLevelItems();
    void OnLevelSelected(const FLevelData& Data);
    UAGameInstance* GetAGameInstance() const;
};
