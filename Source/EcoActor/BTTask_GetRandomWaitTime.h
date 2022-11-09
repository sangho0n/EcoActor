// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GetRandomWaitTime.generated.h"

/**
 * 
 */
UCLASS()
class ECOACTOR_API UBTTask_GetRandomWaitTime : public UBTTaskNode
{
	GENERATED_BODY()

	UBTTask_GetRandomWaitTime();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerCompoent, uint8* NodeMemory) override;
};
