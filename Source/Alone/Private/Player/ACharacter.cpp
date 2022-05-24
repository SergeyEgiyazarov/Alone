// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/ACharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ACharacterMovementComponent.h"

AACharacter::AACharacter(const FObjectInitializer& ObjInit)
	: Super(ObjInit.SetDefaultSubobjectClass<UACharacterMovementComponent>(AACharacter::CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SocketOffset = FVector(0.0f, 100.0f, 80.0f);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

// Called when the game starts or when spawned
void AACharacter::BeginPlay()
{
	Super::BeginPlay();
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
