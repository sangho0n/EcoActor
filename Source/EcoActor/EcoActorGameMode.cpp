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
		// 위젯을 생성한다
		StartWidget = Cast<UEcoActorStartWidget>(CreateWidget(GetWorld(), GameStartWidgetClass));

		if (IsValid(StartWidget))
		{
			// 위젯을 뷰포트에 추가한다
			StartWidget->AddToViewport();
			
			// 마우스 화면에 보이기
			auto palyerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			palyerController->SetInputMode(FInputModeGameAndUI());
			palyerController->bShowMouseCursor = true;
			palyerController->bEnableClickEvents = true;
			palyerController->bEnableMouseOverEvents = true;
		}
	}
}