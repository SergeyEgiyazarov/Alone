// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ACharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UAHealthComponent;
class UTextRenderComponent;

UCLASS()
class ALONE_API AACharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AACharacter(const FObjectInitializer& ObjInit);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UAHealthComponent* HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UTextRenderComponent* HealthText;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* DeathAnimMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    FVector2D LandedDamageVelocity = FVector2D(600.0f, 1200.0f);

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    FVector2D LandedDamage = FVector2D(10.0f, 100.0f);

    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    bool IsRunning() const;

private:
    bool WantsToRun = false;

    void MoveForward(float Amount);
    void MoveRight(float Amount);

    void StartRunning();
    void StopRunning();

    void OnDeath();
    void OnHealthChanged(float Health);

    UFUNCTION()
    void OnGroundLanded(const FHitResult& Hit);
};
