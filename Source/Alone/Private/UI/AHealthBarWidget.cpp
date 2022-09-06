// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/AHealthBarWidget.h"
#include "Components/ProgressBar.h"

void UAHealthBarWidget::SetHealthPercent(float Percent)
{
    if (!HealthProgressBar) return;
    const auto HealthBarVisibility = (Percent > PercentVisibilityThreshold || FMath::IsNearlyZero(Percent)) //
                                         ? ESlateVisibility::Hidden                                         //
                                         : ESlateVisibility::Visible;
    HealthProgressBar->SetVisibility(HealthBarVisibility);

    const auto HealtBarColor = Percent > PercentColorThreshold ? GoodColor : BadColor;
    HealthProgressBar->SetFillColorAndOpacity(HealtBarColor);

    HealthProgressBar->SetPercent(Percent);
}
