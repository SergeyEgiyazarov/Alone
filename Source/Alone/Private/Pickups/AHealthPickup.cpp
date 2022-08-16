// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickups/AHealthPickup.h"

DEFINE_LOG_CATEGORY_STATIC(HealthPickupLog, All, All);

bool AAHealthPickup::GivePickupTo(APawn* PlayerPawn)
{
    UE_LOG(HealthPickupLog, Display, TEXT("Health pickup was taken"));
    return true;
}