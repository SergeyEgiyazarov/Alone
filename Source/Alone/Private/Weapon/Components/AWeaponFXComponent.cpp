// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/Components/AWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

UAWeaponFXComponent::UAWeaponFXComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UAWeaponFXComponent::PlayImpactFX(const FHitResult& Hit)
{
    auto Effect = DefaultEffect;

    if (Hit.PhysMaterial.IsValid())
    {
        const auto PhysMat = Hit.PhysMaterial.Get();
        if (EffectsMap.Contains(PhysMat))
        {
            Effect = EffectsMap[PhysMat];
        }
    }

    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Effect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
}
