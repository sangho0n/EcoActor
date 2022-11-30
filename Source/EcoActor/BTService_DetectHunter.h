// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EcoActor.h"
#include "BehaviorTree/BTService.h"
#include "BTService_DetectHunter.generated.h"

/**
 * 
 */
UCLASS()
class ECOACTOR_API UBTService_DetectHunter : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_DetectHunter();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
