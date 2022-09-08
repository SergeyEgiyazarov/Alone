// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/APlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/AWeaponComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"

AAPlayerCharacter::AAPlayerCharacter(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->SocketOffset = FVector(0.0f, 100.0f, 80.0f);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
    CameraComponent->SetupAttachment(SpringArmComponent);

    CameraCollisionComponent = CreateDefaultSubobject<USphereComponent>("CameraCollisionComponent");
    CameraCollisionComponent->SetupAttachment(CameraComponent);
    CameraCollisionComponent->SetSphereRadius(10.0f);
    CameraCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}


void AAPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(CameraCollisionComponent);
    CameraCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AAPlayerCharacter::OnCameraCollisionBeginOverlap);
    CameraCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AAPlayerCharacter::OnCameraCollisionEndOverlap);
}

void AAPlayerCharacter::OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    CheckCameraOverlap();
}


void AAPlayerCharacter::OnCameraCollisionEndOverlap(
    UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    CheckCameraOverlap();
}

// Called to bind functionality to input
void AAPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    check(WeaponComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &AAPlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AAPlayerCharacter::MoveRight);
    PlayerInputComponent->BindAxis("TurnAround", this, &AAPlayerCharacter::AddControllerYawInput);
    PlayerInputComponent->BindAxis("LookUp", this, &AAPlayerCharacter::AddControllerPitchInput);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AAPlayerCharacter::Jump);
    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AAPlayerCharacter::StartRunning);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &AAPlayerCharacter::StopRunning);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &UAWeaponComponent::StartFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &UAWeaponComponent::StopFire);
    PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &UAWeaponComponent::NextWeapon);
    PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &UAWeaponComponent::Reload);

    DECLARE_DELEGATE_OneParam(FZoomInputSignature, bool);
    PlayerInputComponent->BindAction<FZoomInputSignature>("Zoom", IE_Pressed, WeaponComponent, &UAWeaponComponent::Zoom, true);
    PlayerInputComponent->BindAction<FZoomInputSignature>("Zoom", IE_Released, WeaponComponent, &UAWeaponComponent::Zoom, false);
}

void AAPlayerCharacter::MoveForward(float Amount)
{
    AddMovementInput(GetActorForwardVector(), Amount);
}

void AAPlayerCharacter::MoveRight(float Amount)
{
    AddMovementInput(GetActorRightVector(), Amount);
}

void AAPlayerCharacter::StartRunning()
{
    WantsToRun = true;
}

void AAPlayerCharacter::StopRunning()
{
    WantsToRun = false;
}

bool AAPlayerCharacter::IsRunning() const
{
    return WantsToRun && !GetVelocity().IsZero();
}

void AAPlayerCharacter::OnDeath()
{
    Super::OnDeath();

    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }
}

void AAPlayerCharacter::CheckCameraOverlap()
{
    const auto HideMesh = CameraCollisionComponent->IsOverlappingComponent(GetCapsuleComponent());
    GetMesh()->SetOwnerNoSee(HideMesh);

    TArray<USceneComponent*> MeshChildren;
    GetMesh()->GetChildrenComponents(true, MeshChildren);

    for (const auto MeshChild : MeshChildren)
    {
        const auto MeshChildGeometry = Cast<UPrimitiveComponent>(MeshChild);
        if (MeshChildGeometry)
        {
            MeshChildGeometry->SetOwnerNoSee(HideMesh);
        }
    }
}
