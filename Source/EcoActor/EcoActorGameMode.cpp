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
		// ������ �����Ѵ�
		GameStartWidget = Cast<UEcoActorStartWidget>(CreateWidget(GetWorld(), GameStartWidgetClass));

		if (IsValid(GameStartWidget))
		{
			// ������ ����Ʈ�� �߰��Ѵ�
			GameStartWidget->AddToViewport();
		}
	}
	*/
}