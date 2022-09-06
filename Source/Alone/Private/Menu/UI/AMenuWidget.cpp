// Fill out your copyright notice in the Description page of Project Settings.

#include "Menu/UI/AMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "AGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/HorizontalBox.h"
#include "Menu/UI/ALevelItemWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogAGameInstance, All, All);

void UAMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (StartGameButton)
    {
        StartGameButton->OnClicked.AddDynamic(this, &UAMenuWidget::OnStartGame);
    }

    if (QuitGameButton)
    {
        QuitGameButton->OnClicked.AddDynamic(this, &UAMenuWidget::OnQuitGame);
    }

    InitLevelItems();
}

void UAMenuWidget::InitLevelItems()
{
    const auto AGameInstance = GetAGameInstance();
    if (!AGameInstance) return;

    checkf(AGameInstance->GetLevelsData().Num() != 0, TEXT("Levels data must not be empty!"));

    if (!LevelItemsBox) return;
    LevelItemsBox->ClearChildren();

    for (auto LevelData : AGameInstance->GetLevelsData())
    {
        const auto LevelItemWidget = CreateWidget<UALevelItemWidget>(GetWorld(), LevelItemWidgetClass);
        if (!LevelItemWidget) continue;

        LevelItemWidget->SetLevelData(LevelData);
        LevelItemWidget->OnLevelSelected.AddUObject(this, &UAMenuWidget::OnLevelSelected);

        LevelItemsBox->AddChild(LevelItemWidget);
        LevelItemWidgets.Add(LevelItemWidget);
    }

    if (AGameInstance->GetStartupLevel().LevelName.IsNone())
    {
        OnLevelSelected(AGameInstance->GetLevelsData()[0]);
    }
    else
    {
        OnLevelSelected(AGameInstance->GetStartupLevel());
    }
}

void UAMenuWidget::OnLevelSelected(const FLevelData& Data)
{
    const auto AGameInstance = GetAGameInstance();
    if (!AGameInstance) return;

    AGameInstance->SetStartupLevel(Data);

    for (auto LevelItemWidget : LevelItemWidgets)
    {
        if (LevelItemWidget)
        {
            const auto IsSelected = Data.LevelName == LevelItemWidget->GetLevelData().LevelName;
            LevelItemWidget->SetSelected(IsSelected);
        }
    }
}

void UAMenuWidget::OnStartGame()
{
    PlayAnimation(HideAnimation);
}

void UAMenuWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    if (Animation != HideAnimation) return;

    const auto AGameInstance = GetAGameInstance();
    if (!AGameInstance) return;

    UGameplayStatics::OpenLevel(this, AGameInstance->GetStartupLevel().LevelName);
}

void UAMenuWidget::OnQuitGame()
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}

UAGameInstance* UAMenuWidget::GetAGameInstance() const
{
    if (!GetWorld()) return nullptr;
    return GetWorld()->GetGameInstance<UAGameInstance>();
}

