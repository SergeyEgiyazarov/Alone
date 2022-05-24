// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/ACharacterMovementComponent.h"
#include "Player/ACharacter.h"

float UACharacterMovementComponent::GetMaxSpeed() const
{
	const float MaxSpeed = Super::GetMaxSpeed();
	const AACharacter* Player = Cast<AACharacter>(GetPawnOwner());
	return Player && Player->IsRunning() ? MaxSpeed * RunModifier : MaxSpeed;
}
