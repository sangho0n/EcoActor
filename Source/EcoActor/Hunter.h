// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EcoActor.h"
#include "GameFramework/Character.h"
#include "EcoActorCharacter.h"
#include "Hunter.generated.h"

UCLASS()
class ECOACTOR_API AHunter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHunter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	virtual void PostInitializeComponents() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

private:
	class AEcoActorCharacter* Player;
	bool bAttacked;

public:
	void SetPlayer();
	bool GetAttacked();
	void SetAttacked();
};
