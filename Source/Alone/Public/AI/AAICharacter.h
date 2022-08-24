// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/ACharacter.h"
#include "AAICharacter.generated.h"

class UBehaviorTree;

UCLASS()
class ALONE_API AAAICharacter : public AACharacter
{
    GENERATED_BODY()

public:
    AAAICharacter(const FObjectInitializer& ObjInit);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    UBehaviorTree* BehaviorTreeAsset;
};
