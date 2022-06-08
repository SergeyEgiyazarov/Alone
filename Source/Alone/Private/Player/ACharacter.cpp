// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/ACharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ACharacterMovementComponent.h"
#include "Components/AHealthComponent.h"
#include "Components/TextRenderComponent.h"

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
}

// Called when the game starts or when spawned
void AACharacter::BeginPlay()
{
    Super::BeginPlay();

    check(HealthComponent);
    check(HealthText);

    OnTakeAnyDamage.AddDynamic(this, &AACharacter::OnTakeAnyDamageHandle);
}

// Called every frame
void AACharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    const auto Health = HealthComponent->GetHealth();
    HealthText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));

    TakeDamage(0.1f, FDamageEvent{}, Controller, this);
}

void AACharacter::OnTakeAnyDamageHandle(
    AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
    UE_LOG(CharacterLog, Display, TEXT("Damage: %f"), Damage);
}

// Called to bind functionality to input
void AACharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &AACharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AACharacter::MoveRight);
    PlayerInputComponent->BindAxis("TurnAround", this, &AACharacter::AddControllerYawInput);
    PlayerInputComponent->BindAxis("LookUp", this, &AACharacter::AddControllerPitchInput);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AACharacter::Jump);
    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AACharacter::StartRunning);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &AACharacter::StopRunning);
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
