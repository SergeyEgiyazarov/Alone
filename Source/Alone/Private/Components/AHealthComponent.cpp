// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/AHealthComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraShakeBase.h"
#include "Engine/World.h"
#include "Alone/AloneGameModeBase.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Perception/AISense_Damage.h"

// Sets default values for this component's properties
UAHealthComponent::UAHealthComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false;
}

void UAHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    check(MaxHealth > 0.0f);

    SetHealth(MaxHealth);

    AActor* ComponentOwner = GetOwner();
    if (ComponentOwner)
    {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UAHealthComponent::OnTakeAnyDamage);
        ComponentOwner->OnTakePointDamage.AddDynamic(this, &UAHealthComponent::OnTakePointDamage);
        ComponentOwner->OnTakeRadialDamage.AddDynamic(this, &UAHealthComponent::OnTakeRadialDamage);
    }
}

void UAHealthComponent::OnTakeAnyDamage(
    AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
}

void UAHealthComponent::OnTakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation,
    class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType,
    AActor* DamageCauser)
{
    const auto FinalDamage = Damage * GetPointDamageModifier(DamagedActor, BoneName);
    ApplyDamage(FinalDamage, InstigatedBy);
}

void UAHealthComponent::OnTakeRadialDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin,
    FHitResult HitInfo, class AController* InstigatedBy, AActor* DamageCauser)
{
    ApplyDamage(Damage, InstigatedBy);
}

void UAHealthComponent::ApplyDamage(float Damage, AController* InstigatedBy)
{
    if (Damage <= 0.0f || IsDead() || !GetWorld()) return;

    SetHealth(Health - Damage);

    GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);

    if (IsDead())
    {
        Killed(InstigatedBy);
        OnDeath.Broadcast();
    }
    else if (AutoHeal)
    {
        GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &UAHealthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
    }

    PlayCameraShake();
    ReportDamageEvent(Damage, InstigatedBy);
}

float UAHealthComponent::GetPointDamageModifier(AActor* DamagedActor, const FName& BoneName)
{
    const auto Character = Cast<ACharacter>(DamagedActor);
    if (!Character ||            //
        !Character->GetMesh() || //
        !Character->GetMesh()->GetBodyInstance(BoneName))
        return 1.0f;

    const auto PhysicalMaterial = Character->GetMesh()->GetBodyInstance(BoneName)->GetSimplePhysicalMaterial();
    if (!PhysicalMaterial || !DamageModifiers.Contains(PhysicalMaterial)) return 1.0f;

    return DamageModifiers[PhysicalMaterial];
}

void UAHealthComponent::HealUpdate()
{
    SetHealth(Health + HealModifier);

    if (FMath::IsNearlyEqual(Health, MaxHealth) && GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
    }
}

void UAHealthComponent::SetHealth(float NewHealth)
{
    const auto NextHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    const auto HealthDelta = NextHealth - Health;

    Health = NextHealth;
    OnHealthChanged.Broadcast(Health, HealthDelta);
}

bool UAHealthComponent::TryToAddHealth(float HealthAmount)
{
    if (IsDead() || IsHealthFull()) return false;

    SetHealth(Health + HealthAmount);
    return true;
}

bool UAHealthComponent::IsHealthFull() const
{
    return FMath::IsNearlyEqual(Health, MaxHealth);
}

void UAHealthComponent::PlayCameraShake()
{
    if (IsDead()) return;

    const auto Player = Cast<APawn>(GetOwner());
    if (!Player) return;

    const auto Controller = Player->GetController<APlayerController>();
    if (!Controller || !Controller->PlayerCameraManager) return;

    Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}

void UAHealthComponent::Killed(AController* KillerController)
{
    if (!GetWorld()) return;

    const auto GameMode = Cast<AAloneGameModeBase>(GetWorld()->GetAuthGameMode());
    if (!GameMode) return;

    const auto Player = Cast<APawn>(GetOwner());
    const auto VictimController = Player ? Player->Controller : nullptr;

    GameMode->Killed(KillerController, VictimController);
}

void UAHealthComponent::ReportDamageEvent(float Damage, AController* InstigatedBy)
{
    if (!InstigatedBy || !InstigatedBy->GetPawn() || !GetOwner()) return;

    UAISense_Damage::ReportDamageEvent(GetWorld(),   //
        GetOwner(),                                  //
        InstigatedBy->GetPawn(),                     //
        Damage,                                      //
        InstigatedBy->GetPawn()->GetActorLocation(), //
        GetOwner()->GetActorLocation());
}
