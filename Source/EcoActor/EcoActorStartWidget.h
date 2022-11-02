// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EcoActorStartWidget.generated.h"

/**
 * 
 */
UCLASS()
class ECOACTOR_API UEcoActorStartWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	class UButton* StartBut;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UImage* EarthIcon;

protected:
	virtual void NativeOnInitialized() override;

private:

	UFUNCTION()
	void OnStartButClicked();
};
