// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EcoActor.h"
#include "Animation/AnimInstance.h"
#include "AnimalAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ECOACTOR_API UAnimalAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UAnimalAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Pawn, Meta=(AllowPrivateAccess=true))
	float currSpeed;

};
