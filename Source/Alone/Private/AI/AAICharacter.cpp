// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/AAICharacter.h"
#include "AI/AAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/AAIWeaponComponent.h"

AAAICharacter::AAAICharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<UAAIWeaponComponent>("WeaponComponent"))
{
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    AIControllerClass = AAAIController::StaticClass();

    bUseControllerRotationYaw = false;
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
    }
}
