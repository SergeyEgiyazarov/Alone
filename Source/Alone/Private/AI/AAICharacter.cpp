// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/AAICharacter.h"
#include "AI/AAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/AAIWeaponComponent.h"
#include "BrainComponent.h"

AAAICharacter::AAAICharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<UAAIWeaponComponent>("WeaponComponent"))
{
    AutoPossessAI = EAutoPossessAI::Disabled;
    AIControllerClass = AAAIController::StaticClass();

    bUseControllerRotationYaw = false;
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
    }
}

void AAAICharacter::OnDeath()
{
    Super::OnDeath();

    const auto AIController = Cast<AAAIController>(Controller);
    if (AIController && AIController->BrainComponent)
    {
        AIController->BrainComponent->Cleanup();
    }
}
