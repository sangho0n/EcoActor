// Fill out your copyright notice in the Description page of Project Settings.


#include "Level3CommonUI.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"


void ULevel3CommonUI::BindCharacterStat(UCharacterStat* NewStat)
{
	CurrentCharStat = NewStat;
	CurrentCharStat->OnHPChanged.AddUObject(this, &ULevel3CommonUI::UpdateHPBar);
}

void ULevel3CommonUI::NativeConstruct()
{
	Super::NativeConstruct();
	PB_HP_C = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HP")));
	TEXT_SCORE_C = Cast<UTextBlock>(GetWidgetFromName(TEXT("TEXT_Score")));
	TEXT_LEFT_C = Cast<UTextBlock>(GetWidgetFromName(TEXT("TEXT_Left")));
	Img_Fist_C = Cast<UImage>(GetWidgetFromName(TEXT("Img_Fist")));
	Img_Bullet_C = Cast<UImage>(GetWidgetFromName(TEXT("Img_Bullet")));
	Img_Aim_C = Cast<UImage>(GetWidgetFromName(TEXT("Img_Aim")));
	UpdateHPBar();
}

void ULevel3CommonUI::UpdateHPBar()
{
	if (CurrentCharStat.IsValid())
	{
		if (nullptr != PB_HP_C)
		{
			PB_HP_C->SetPercent(CurrentCharStat->GetHPRatio());
		}
	}
}

void ULevel3CommonUI::UpdateScore()
{
	if (CurrentCharStat.IsValid())
	{
		if (nullptr != TEXT_SCORE_C)
		{
			TEXT_SCORE_C->SetText(FText::FromString(TEXT("Score : ") + FString::FromInt(CurrentCharStat->GetScore())));
		}
	}
}

void ULevel3CommonUI::SetFistMode()
{
	Img_Bullet_C->SetVisibility(ESlateVisibility::Hidden);
	Img_Fist_C->SetVisibility(ESlateVisibility::Visible);
	Img_Aim_C->SetVisibility(ESlateVisibility::Hidden);
	TEXT_LEFT_C->SetText(FText::FromString(TEXT("Infinite")));
}

void ULevel3CommonUI::SetGunMode()
{
	Img_Bullet_C->SetVisibility(ESlateVisibility::Visible);
	Img_Fist_C->SetVisibility(ESlateVisibility::Hidden);
	Img_Aim_C->SetVisibility(ESlateVisibility::Visible);
	TEXT_LEFT_C->SetText(FText::FromString(TEXT("20/20")));
}

void ULevel3CommonUI::UpdateLeftBullet(int32 left, const int32 max)
{
	TEXT_LEFT_C->SetText(FText::FromString(FString::FromInt(left) + TEXT(" / ") + FString::FromInt(max)));
}