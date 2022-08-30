// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/ACharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ACharacterMovementComponent.h"
#include "Components/AHealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/AWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"

DEFINE_LOG_CATEGORY_STATIC(CharacterLog, All, All);

AACharacter::AACharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<UACharacterMovementComponent>(AACharacter::CharacterMovementComponentName))
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->SocketOffset = FVector(0.0f, 100.0f, 80.0f);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
    CameraComponent->SetupAttachment(SpringArmComponent);

    HealthComponent = CreateDefaultSubobject<UAHealthComponent>("HealthComponent");

    HealthText = CreateDefaultSubobject<UTextRenderComponent>("HealthText");
    HealthText->SetupAttachment(GetRootComponent());
    HealthText->SetOwnerNoSee(true);

    WeaponComponent = CreateDefaultSubobject<UAWeaponComponent>("WeaponComponent");
}

// Called when the game starts or when spawned
void AACharacter::BeginPlay()
{
    Super::BeginPlay();

    check(HealthComponent);
    check(HealthText);

    OnHealthChanged(HealthComponent->GetHealth(), 0.0f);
    HealthComponent->OnDeath.AddUObject(this, &AACharacter::OnDeath);
    HealthComponent->OnHealthChanged.AddUObject(this, &AACharacter::OnHealthChanged);

    LandedDelegate.AddDynamic(this, &AACharacter::OnGroundLanded);
}

void AACharacter::OnHealthChanged(float Health, float HealthDelta)
{
    HealthText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

// Called every frame
void AACharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AACharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    check(WeaponComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &AACharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AACharacter::MoveRight);
    PlayerInputComponent->BindAxis("TurnAround", this, &AACharacter::AddControllerYawInput);
    PlayerInputComponent->BindAxis("LookUp", this, &AACharacter::AddControllerPitchInput);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AACharacter::Jump);
    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AACharacter::StartRunning);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &AACharacter::StopRunning);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &UAWeaponComponent::StartFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &UAWeaponComponent::StopFire);
    PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &UAWeaponComponent::NextWeapon);
    PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &UAWeaponComponent::Reload);
}

void AACharacter::MoveForward(float Amount)
{
    AddMovementInput(GetActorForwardVector(), Amount);
}

void AACharacter::MoveRight(float Amount)
{
    AddMovementInput(GetActorRightVector(), Amount);
}

void AACharacter::StartRunning()
{
    WantsToRun = true;
}

void AACharacter::StopRunning()
{
    WantsToRun = false;
}

bool AACharacter::IsRunning() const
{
    return WantsToRun && !GetVelocity().IsZero();
}

void AACharacter::OnDeath()
{
    UE_LOG(CharacterLog, Display, TEXT("Player %s is dead."), *GetName());
    PlayAnimMontage(DeathAnimMontage);
    HealthText->SetVisibility(false);

    GetCharacterMovement()->DisableMovement();
    SetLifeSpan(5.0f);

    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }

    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    WeaponComponent->StopFire();
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
