// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EcoActor.h"
#include "Components/ActorComponent.h"
#include "CharacterStat.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHPIsChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnScoreChangedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnScoreReachTopDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ECOACTOR_API UCharacterStat : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterStat();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetDamage(float NewDam);
	void SetHP(float NewHP);
	void SetMaxHP(float NewMaxHP);
	float GetCurrentHP();
	float GetMaxHP();
	float GetHPRatio();
		
	FOnHPIsZeroDelegate OnHPIsZero;
	FOnHPIsChangedDelegate OnHPChanged;

private:
	float CurrentHP;
	float MaxHP;
	bool bIsDead;
	int64 Score;

public:
	void ScoreUp();
	FOnScoreChangedDelegate OnScoreChanged;
	int64 GetScore();
	FOnScoreReachTopDelegate OnScoreReachTop;
private:
	int64 ScoreMax;
	void EqualsOrGreaterThanScoreMax();
};
