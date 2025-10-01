// Fill out your copyright notice in the Description page of Project Settings.

#include "AgentPlayerController.h"
#include "HackerPlayerController.h"
#include "CoopGame/Core/Puzzle/Utils.h"
#include "CoopGame/Core/GameModes/GameplayGameMode.h"



void AAgentPlayerController::Server_CheckSentCode_Implementation(const TArray<int8>& Array, const GameUserWidget widget)
{
	AGameplayGameMode* GameModeRef = Cast<AGameplayGameMode>(GetWorld()->GetAuthGameMode());
	ACoopGameState* GameStateRef = Cast<ACoopGameState>(GetWorld()->GetGameState());
	//UE_LOG(LogTemp, Warning, TEXT(" Array size Server: %d"), GameStateRef->CodePuzzleSolution.Num());

	if (GameModeRef && GameStateRef)
	{
		switch (widget)
		{
		case GameUserWidget::DigitDisplay:
			if (Utils::CheckEquals(Array, GameStateRef->CodePuzzleSolution))
			{
				Multicast_ShowDebugMessage(TEXT("Door Unlocked")); //Placeholder for unlock door function
			}
			break;
		case GameUserWidget::SoundPuzzleWidget:
			break;
		default:
			break;
		}
	}
}

void AAgentPlayerController::SendArrayCode_Implementation(const TArray<int8>& Array, const GameUserWidget widget)
{
	if (!GetWorld()) return;
	
	AHackerPlayerController* HackerPC = Cast<ACoopGameState>(GetWorld()->GetGameState())->HackerPlayerController;
	if (IsValid(HackerPC))
	{
		HackerPC->ReceiveArrayCode(Array, widget);
	}
	
	//Server_CheckSentCode(Array, widget);
}

void AAgentPlayerController::ServerRequestCheckCode_Implementation(AKeypad* Keypad, const TArray<int8>& SentCode)
{
	if (Keypad)
	{
		Keypad->Server_CheckCode(SentCode); // Server-authoritative
	}
}

void AAgentPlayerController::ServerRequestSaveCodeIntoGameState_Implementation(AKeypad* Keypad)
{
	if (Keypad)
	{
		Keypad->Server_SaveCodeIntoGameState(); // Server-authoritative
	}
}