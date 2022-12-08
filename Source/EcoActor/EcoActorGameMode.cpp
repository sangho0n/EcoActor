// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "EcoActorGameMode.h"
#include "EcoActorCharacter.h"
#include "EcoActorCharacterState.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

AEcoActorGameMode::AEcoActorGameMode()
{
	DefaultPawnClass = AEcoActorCharacter::StaticClass();
	PlayerStateClass = AEcoActorCharacterState::StaticClass();

	static ConstructorHelpers::FClassFinder<UUserWidget>FAILWIDGET(TEXT("/Game/Main/UI/BP_GameOver_UI.BP_GameOver_UI_C"));
	if (FAILWIDGET.Succeeded())
	{
		GameEndWidgetClass = FAILWIDGET.Class;
	}
}

void AEcoActorGameMode::BeginPlay()
{
	Super::BeginPlay();
	auto Player = Cast<AEcoActorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (IsValid(Player))
	{
		Player->OnGameFailed.BindUFunction(this, TEXT("OnFail"));
	}
}

void AEcoActorGameMode::OnFail()
{
	GameEndWidget = CreateWidget(GetWorld(), GameEndWidgetClass);
	if (IsValid(GameEndWidget))
	{
		GameEndWidget->AddToViewport();
	}
}