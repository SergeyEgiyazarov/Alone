// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/AAICharacter.h"
#include "AI/AAIController.h"

AAAICharacter::AAAICharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit)
{
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    AIControllerClass = AAAIController::StaticClass();
}
