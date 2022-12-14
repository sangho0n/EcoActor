// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EcoActor.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

UCLASS()
class ECOACTOR_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	FTimerHandle HunterHandle = {};
	FTimerHandle AnimalHandle = {};
	FTimerHandle GunHandle = {};
	float HunterTime;
	float AnimalTime;
	float GunTime;
	void SpawnHunter();
	void SpawnAnimal();
	void SpawnGun();

	int32 MaxHunter;
	int32 MaxAnimal;
	int32 MaxGun;
	UPROPERTY(VisibleAnywhere)
	int32 CurrHunter;
	UPROPERTY(VisibleAnywhere)
	int32 CurrAnimal;
	UPROPERTY(VisibleAnywhere)
	int32 CurrGun;

public:
	void DecCurrHunter();
	void DecCurrAnimal();
	void DecCurrGun();
};
