// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat.h"

// Sets default values for this component's properties
UCharacterStat::UCharacterStat()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	bIsDead = false;
	Score = 0;
	ScoreMax = 3;
}


// Called when the game starts
void UCharacterStat::BeginPlay()
{
	Super::BeginPlay();

	SetHP(MaxHP);
	Score = 0;
}

void UCharacterStat::InitializeComponent()
{
	Super::InitializeComponent();
	SetHP(MaxHP);
}

void UCharacterStat::SetDamage(float NewDam)
{
	SetHP(FMath::Clamp<float>(CurrentHP - NewDam, 0.0f, MaxHP));
}

void UCharacterStat::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	OnHPChanged.Broadcast();
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.0f;
		OnHPIsZero.Broadcast();
	}
}

void UCharacterStat::SetMaxHP(float NewMax)
{
	MaxHP = NewMax;
	SetHP(MaxHP);
}

float UCharacterStat::GetHPRatio()
{
	return CurrentHP / MaxHP;
}

void UCharacterStat::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

float UCharacterStat::GetCurrentHP()
{
	return CurrentHP;
}

float UCharacterStat::GetMaxHP()
{
	return MaxHP;
}

void UCharacterStat::ScoreUp()
{
	Score++;
	OnScoreChanged.Broadcast();
}

int64 UCharacterStat::GetScore()
{
	return Score;
}

void UCharacterStat::EqualsOrGreaterThanScoreMax()
{
	OnScoreReachTop.Broadcast();
}