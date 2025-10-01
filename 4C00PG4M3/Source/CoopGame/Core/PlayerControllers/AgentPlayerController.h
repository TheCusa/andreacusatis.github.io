// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoopPlayerController.h"
#include "CoopGame/FirstPuzzle/Keypad.h"
#include "AgentPlayerController.generated.h"

UCLASS()
class COOPGAME_API AAgentPlayerController : public ACoopPlayerController
{
	GENERATED_BODY()

public: 

	UFUNCTION(Server, Reliable)
	
	void SendArrayCode(const TArray<int8>& Array, const GameUserWidget widget );

	UFUNCTION(Server, Reliable)
	void ServerRequestCheckCode(AKeypad* Keypad, const TArray<int8>& SentCode);

	UFUNCTION(Server, Reliable)
	void ServerRequestSaveCodeIntoGameState(AKeypad* Keypad);

private:
	UFUNCTION(Server, Reliable)
	void Server_CheckSentCode(const TArray<int8>& Array, const GameUserWidget widget);


};
