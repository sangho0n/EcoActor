// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EcoActor.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "HunterHPWidget.generated.h"

/**
 * 
 */
UCLASS()
class ECOACTOR_API UHunterHPWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindHunterStat(class UCharacterStat* NewStat);
	virtual void NativeConstruct() override;
private:
	TWeakObjectPtr<class UCharacterStat> CurrentCharacterStat;
public:
	class UProgressBar* PB_HP_C;
};
