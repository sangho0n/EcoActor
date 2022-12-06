// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EcoActor.h"
#include "AIController.h"
#include "HunterAiController.generated.h"

/**
 * 
 */
UCLASS()
class ECOACTOR_API AHunterAiController : public AAIController
{
	GENERATED_BODY()
public:
	AHunterAiController();

	static const FName HomePosKey;

	static const FName PatrolPosKey;

	static const FName RandomWaitTimeKey;

	static const FName DetectedAnimalKey;

	static const FName DetectedAnimalPosKey;

	static const FName WasAttackedKey;

	static const FName PlayerPosKey;

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY()
	class UBlackboardData* BBAsset;
	UPROPERTY()
	class UBehaviorTree* BTAsset;

	class AHunter* ControlledPawn;

public:
	class AEcoActorCharacter* Player;
};
