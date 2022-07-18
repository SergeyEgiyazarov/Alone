// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/AProjectile.h"
#include "Components/SphereComponent.h"

AAProjectile::AAProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    CollisionComponent->InitSphereRadius(5.0f);
    SetRootComponent(CollisionComponent);
}

void AAProjectile::BeginPlay()
{
    Super::BeginPlay();
}
