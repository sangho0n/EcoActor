// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EcoActor.h"
#include "GameFramework/GameModeBase.h"
#include "EcoActorStartWidget.h"
#include "EcoActorGameMode.generated.h"

UCLASS(minimalapi)
class AEcoActorGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AEcoActorGameMode();

private:
	void BeginPlay() override;

	TSubclassOf<UUserWidget> GameStartWidgetClass;
	UEcoActorStartWidget* GameStartWidget;
};



