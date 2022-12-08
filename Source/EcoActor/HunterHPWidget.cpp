// Fill out your copyright notice in the Description page of Project Settings.


#include "HunterHPWidget.h"
#include "CharacterStat.h"

void UHunterHPWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	PB_HP_C = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HP")));
}
void UHunterHPWidget::BindHunterStat(UCharacterStat* NewStat)
{
	CurrentCharacterStat = NewStat;

	CurrentCharacterStat->OnHPChanged.AddLambda([this]()->void {
		PB_HP_C->SetPercent(CurrentCharacterStat->GetHPRatio());
		});
}