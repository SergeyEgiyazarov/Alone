// Fill out your copyright notice in the Description page of Project Settings.

#include "Menu/UI/ALevelItemWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UALevelItemWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (LevelSelectButton)
    {
        LevelSelectButton->OnClicked.AddDynamic(this, &UALevelItemWidget::OnLevelItemClicked);
    }
}

void UALevelItemWidget::OnLevelItemClicked()
{
    OnLevelSelected.Broadcast(LevelData);
}

void UALevelItemWidget::SetLevelData(const FLevelData& Data)
{
    LevelData = Data;

    if (LevelNameTextBlock)
    {
        LevelNameTextBlock->SetText(FText::FromName(Data.LevelDisplayName));
    }

    if (LevelImage)
    {
        LevelImage->SetBrushFromTexture(Data.LevelTumb);
    }
}

void UALevelItemWidget::SetSelected(bool IsSelected)
{
    if (FrameImage)
    {
        FrameImage->SetVisibility(IsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}

