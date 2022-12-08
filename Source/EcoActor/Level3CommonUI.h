// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EcoActor.h"
#include "UserWidget.h"
#include "CharacterStat.h"
#include "Level3CommonUI.generated.h"

/**
 * 
 */
UCLASS()
class ECOACTOR_API ULevel3CommonUI : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void BindCharacterStat(class UCharacterStat* NewCharacterStat);

protected:
	virtual void NativeConstruct() override;

	void UpdateHPBar();

private:
	TWeakObjectPtr<class UCharacterStat> CurrentCharStat;

	UPROPERTY()
	class UProgressBar* PB_HP_C;
	class UTextBlock* TEXT_SCORE_C;
	class UTextBlock* TEXT_LEFT_C;
	class UImage* Img_Fist_C;
	class UImage* Img_Bullet_C;
	class UImage* Img_Aim_C;
	class UTextBlock* TEXT_TimeLeft_C;

public:
	void SetFistMode();
	void SetGunMode();
	void UpdateLeftBullet(int32 leftBullet, const int32 maxBullet);
	void UpdateScore();
	void UpdateLeftTime();
public:
	void PlayTimer();

private:
	int32 min;
	int32 sec;
};
