// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EcoActor.h"
#include "GameFramework/Character.h"
#include "EcoActorCrocodile.generated.h"

UCLASS()
class ECOACTOR_API AEcoActorCrocodile : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEcoActorCrocodile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
