// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/ACharacter.h"
#include "APlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class ALONE_API AAPlayerCharacter : public AACharacter
{
    GENERATED_BODY()

public:
    AAPlayerCharacter(const FObjectInitializer& ObjInit);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* CameraComponent;

    virtual void OnDeath() override;

public:
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual bool IsRunning() const override;

private:
    bool WantsToRun = false;

    void MoveForward(float Amount);
    void MoveRight(float Amount);

    void StartRunning();
    void StopRunning();
};
