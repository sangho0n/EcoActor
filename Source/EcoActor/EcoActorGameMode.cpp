// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "EcoActorGameMode.h"
#include "EcoActorCharacter.h"
#include "UObject/ConstructorHelpers.h"

AEcoActorGameMode::AEcoActorGameMode()
{
	
	DefaultPawnClass = AEcoActorCharacter::StaticClass();
	/*
	ConstructorHelpers::FClassFinder<UUserWidget> STARTUI(TEXT("/Game/Main/UI/StartWidget.StartWidget_C"));
	if (STARTUI.Succeeded())
	{
		GameStartWidgetClass = STARTUI.Class;
	}
	*/
}

void AEcoActorGameMode::BeginPlay()
{
	Super::BeginPlay();
	/*
	if (IsValid(GameStartWidgetClass))
	{
		// 위젯을 생성한다
		GameStartWidget = Cast<UEcoActorStartWidget>(CreateWidget(GetWorld(), GameStartWidgetClass));

		if (IsValid(GameStartWidget))
		{
			// 위젯을 뷰포트에 추가한다
			GameStartWidget->AddToViewport();
		}
	}
	*/
}