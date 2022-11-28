// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EcoActor.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class ECOACTOR_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	virtual void PostInitializeComponents() override;

public:
	UPROPERTY(VisibleAnywhere, Category=Weapon)
	USkeletalMeshComponent* Gun;

private:
	UFUNCTION()
	void OnCharacterBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UBoxComponent* Trigger;
};
