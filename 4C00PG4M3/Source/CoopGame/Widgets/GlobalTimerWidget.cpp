// Fill out your copyright notice in the Description page of Project Settings.


#include "GlobalTimerWidget.h"
#include "Components/TextBlock.h"
#include "CoopGame/Core/CoopGameState.h"
#include "CoopGame/Core/GameModes/GameplayGameMode.h"


void UGlobalTimerWidget::NativeConstruct()
{
	Super::NativeConstruct();
	FText Text;
	TimerMinutes->SetText(Text);
	TimerSeconds->SetText(Text);
	GameStateRef = Cast<ACoopGameState>(GetWorld()->GetGameState());
	NumberFormat.MinimumIntegralDigits = 2;
}

void UGlobalTimerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
		
	if (GameStateRef)
	{
		int32 GameTimerMinutes =  GameStateRef->GameTimer / 60;
		int32 GameTimerSeconds = GameStateRef->GameTimer % 60;
		FText Minutes = FText::AsNumber(GameTimerMinutes, &NumberFormat);
		FText Seconds = FText::AsNumber(GameTimerSeconds, &NumberFormat);
		TimerMinutes->SetText(Minutes);
		TimerSeconds->SetText(Seconds);
	}
}
