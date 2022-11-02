// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EcoActor.h"
#include "GameFramework/Character.h"
#include "EcoActorCharacter.generated.h"

UCLASS(config=Game)
class AEcoActorCharacter : public ACharacter
{
	GENERATED_BODY()


	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AEcoActorCharacter();

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	virtual void Tick(float deltaSeconds) override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:
	void Jump() override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	void Attack();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

private:
	enum EGameMode
	{
		ThirdPerson,
		FirstPerson
	};
public:
	void setPlayerMode(EGameMode gameMode);

private:
	// combo ฐüทร
	int32 currentCombo;
	bool bCanNextCombo;
	bool bIsAttacking;
	bool bIsComboInputOn;
	bool bIsEquipped;
	bool bHoldKeyControl;

	class UEcoActorCharacterAnimInstance* AnimInstance;
protected:
	UFUNCTION()
	void ComboHit();

private:
	UFUNCTION()
	void OnComboMontageEnded(UAnimMontage* Montage, bool bInterupted);
};

