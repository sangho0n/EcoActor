// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "EcoActorGameMode.h"
#include "EcoActorCharacter.h"
#include "EcoActorCharacterState.h"
#include "UObject/ConstructorHelpers.h"

AEcoActorGameMode::AEcoActorGameMode()
{
	DefaultPawnClass = AEcoActorCharacter::StaticClass();
	PlayerStateClass = AEcoActorCharacterState::StaticClass();
	
}

void AEcoActorGameMode::BeginPlay()
{
	Super::BeginPlay();
	
}