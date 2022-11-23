// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RotateToPatrol.generated.h"


/**
 * 
 */
UCLASS()
class ECOACTOR_API UBTTask_RotateToPatrol : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_RotateToPatrol();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerCompoent, uint8* NodeMemory) override;
};
