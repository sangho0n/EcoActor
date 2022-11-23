// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EcoActor.h"
#include "AIController.h"
#include "AnimalAIController.generated.h"

/**
 * 
 */
UCLASS()
class ECOACTOR_API AAnimalAIController : public AAIController
{
	GENERATED_BODY()

public:
	AAnimalAIController();

	virtual void OnPossess(APawn* InPawn) override;

	static const FName HomePosKey;

	static const FName PatrolPosKey;

	static const FName RandomWaitTimeKey;

private:
	UPROPERTY()
	class UBehaviorTree* BTAsset;
	UPROPERTY()
	class UBlackboardData* BBAsset;
};
