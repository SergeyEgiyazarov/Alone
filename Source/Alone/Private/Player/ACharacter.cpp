// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/ACharacter.h"
#include "Components/ACharacterMovementComponent.h"
#include "Components/AHealthComponent.h"
#include "Components/AWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(CharacterLog, All, All);

AACharacter::AACharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<UACharacterMovementComponent>(AACharacter::CharacterMovementComponentName))
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    HealthComponent = CreateDefaultSubobject<UAHealthComponent>("HealthComponent");
    WeaponComponent = CreateDefaultSubobject<UAWeaponComponent>("WeaponComponent");
}

// Called when the game starts or when spawned
void AACharacter::BeginPlay()
{
    Super::BeginPlay();

    check(HealthComponent);

    OnHealthChanged(HealthComponent->GetHealth(), 0.0f);
    HealthComponent->OnDeath.AddUObject(this, &AACharacter::OnDeath);
    HealthComponent->OnHealthChanged.AddUObject(this, &AACharacter::OnHealthChanged);

    LandedDelegate.AddDynamic(this, &AACharacter::OnGroundLanded);
}

void AACharacter::OnHealthChanged(float Health, float HealthDelta)
{
}

// Called every frame
void AACharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

bool AACharacter::IsRunning() const
{
    return false;
}

void AACharacter::OnDeath()
{
    UE_LOG(CharacterLog, Display, TEXT("Player %s is dead."), *GetName());
    PlayAnimMontage(DeathAnimMontage);

    GetCharacterMovement()->DisableMovement();
    SetLifeSpan(5.0f);

    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    WeaponComponent->StopFire();

    UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation());
}

void AACharacter::OnGroundLanded(const FHitResult& Hit)
{
    const auto FallVelocityZ = -GetCharacterMovement()->Velocity.Z;

    if (FallVelocityZ < LandedDamageVelocity.X) return;
    const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);

    TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}

void AACharacter::SetPlayerColor(const FLinearColor& Color)
{
    const auto MaterialInst = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
    if (!MaterialInst) return;

    MaterialInst->SetVectorParameterValue(MaterialColorName, Color);
}
