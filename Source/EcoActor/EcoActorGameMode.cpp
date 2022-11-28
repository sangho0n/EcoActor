// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "EcoActorGameMode.h"
#include "EcoActorCharacter.h"
#include "UObject/ConstructorHelpers.h"

AEcoActorGameMode::AEcoActorGameMode()
{
	DefaultPawnClass = AEcoActorCharacter::StaticClass();
	
	ConstructorHelpers::FClassFinder<UUserWidget> COMMONUI(TEXT("/Game/Main/UI/BP_Level3CommonUI.BP_Level3CommonUI_C"));
	if (COMMONUI.Succeeded())
	{
		CommonUIClass = COMMONUI.Class;
	}
}

void AEcoActorGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(CommonUIClass))
	{
		// 위젯을 생성한다
		CommonUI = Cast<ULevel3CommonUI>(CreateWidget(GetWorld(), CommonUIClass));

		if (IsValid(CommonUI))
		{
			// 위젯을 뷰포트에 추가한다
			CommonUI->AddToViewport();
			
			auto palyerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			palyerController->SetInputMode(FInputModeGameOnly());
			palyerController->bShowMouseCursor = false;
			palyerController->bEnableClickEvents = false;
			palyerController->bEnableMouseOverEvents = false;
		}
	}
}