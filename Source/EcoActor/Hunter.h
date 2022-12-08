// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EcoActor.h"
#include "GameFramework/Character.h"
#include "EcoActorCharacter.h"
#include "CharacterStat.h"
#include "HunterAnimInstance.h"
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
	class UHunterAnimInstance* AnimInstance;
	class UCharacterStat* CharacterStat;
public:
private:
	class UWidgetComponent* HPBarWidget;

	// character State
	ECharacterState HunterState;
	UFUNCTION()
	void SetHunterState(ECharacterState NewState);
	

public:
	UFUNCTION()
	void Attack();
	UFUNCTION()
	void SetDead();

private:
	float AttackRange;
	float AttackRadius;
	float HunterAttackDamage;
	bool bIsDead;
	int32 DeadSecCount;
};
