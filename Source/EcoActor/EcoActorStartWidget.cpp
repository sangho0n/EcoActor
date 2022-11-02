// Fill out your copyright notice in the Description page of Project Settings.


#include "EcoActorStartWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "EcoActorGameMode.h"

void UEcoActorStartWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	StartBut = Cast<UButton>(GetWidgetFromName(TEXT("BP_StartBut")));
	EarthIcon = Cast<UImage>(GetWidgetFromName(TEXT("BP_EarthIcon")));

	StartBut->OnClicked.AddDynamic(this, &UEcoActorStartWidget::OnStartButClicked);
}

void UEcoActorStartWidget::OnStartButClicked()
{
	this->SetVisibility(ESlateVisibility::Hidden);

	// 마우스 숨기기
	auto palyerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	palyerController->SetInputMode(FInputModeGameOnly());
	palyerController->bShowMouseCursor = false;
	palyerController->bEnableClickEvents = false;
	palyerController->bEnableMouseOverEvents = false;
}