// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GlobalTimerWidget.generated.h"


class UTextBlock;
class ACoopGameState;
/**
 * 
 */
UCLASS()
class COOPGAME_API UGlobalTimerWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TimerMinutes;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TimerSeconds;

	UPROPERTY()
	TObjectPtr<ACoopGameState> GameStateRef;

	FNumberFormattingOptions NumberFormat;
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
};
