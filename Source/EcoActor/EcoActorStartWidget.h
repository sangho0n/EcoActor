// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EcoActor.h"
#include "Components/Widget.h"
#include "EcoActorStartWidget.generated.h"

/**
 * 
 */
UCLASS()
class ECOACTOR_API UEcoActorStartWidget : public UWidget
{
	GENERATED_BODY()
private:
	UEcoActorStartWidget();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UButton* StoryButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UButton* StartButton;

	class UImage* EarthImage;
};
