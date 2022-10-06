// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EcoActor.h"
#include "Animation/AnimInstance.h"
#include "EcoActorCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ECOACTOR_API UEcoActorCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UEcoActorCharacterAnimInstance();
	virtual void NativeUpdateAnimation(float deltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta=(AllowPrivateAccess = true))
	float CurrSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bInAir;
};
