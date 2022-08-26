// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/EQS/AEnemyEnvQueryContext.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

void UAEnemyEnvQueryContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
    const auto QueryOwner = Cast<AActor>(QueryInstance.Owner.Get());

    const auto Blackboard = UAIBlueprintHelperLibrary::GetBlackboard(QueryOwner);
    if (!Blackboard) return;

    const auto ContextEnemyActor = Blackboard->GetValueAsObject(EnemyActorKeyName);
    if (!ContextEnemyActor) return;
    UEnvQueryItemType_Actor::SetContextHelper(ContextData, Cast<AActor>(ContextEnemyActor));
}
