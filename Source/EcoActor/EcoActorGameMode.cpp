// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "EcoActorGameMode.h"
#include "EcoActorCharacter.h"
#include "UObject/ConstructorHelpers.h"

AEcoActorGameMode::AEcoActorGameMode()
{
	DefaultPawnClass = AEcoActorCharacter::StaticClass();
	
	ConstructorHelpers::FClassFinder<UUserWidget> STARTUI(TEXT("/Game/Main/UI/BP_StartWidget.BP_StartWidget_C"));
	if (STARTUI.Succeeded())
	{
		GameStartWidgetClass = STARTUI.Class;
	}
	
}

void AEcoActorGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(GameStartWidgetClass))
	{
		// ������ �����Ѵ�
		StartWidget = Cast<UEcoActorStartWidget>(CreateWidget(GetWorld(), GameStartWidgetClass));

		if (IsValid(StartWidget))
		{
			// ������ ����Ʈ�� �߰��Ѵ�
			StartWidget->AddToViewport();
			
			// ���콺 ȭ�鿡 ���̱�
			auto palyerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			palyerController->SetInputMode(FInputModeGameAndUI());
			palyerController->bShowMouseCursor = true;
			palyerController->bEnableClickEvents = true;
			palyerController->bEnableMouseOverEvents = true;
		}
	}
}