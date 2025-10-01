// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "CoopGame/Core/CoopGameState.h"
#include "CoopGame/EOS/EOSGameInstance.h"

ALobbyGameMode::ALobbyGameMode()
{
	bUseSeamlessTravel = true;
}

void ALobbyGameMode::InitGameState()
{
	Super::InitGameState();
	ACoopGameState* CoopGameState = Cast<ACoopGameState>(GameState);
	UEOSGameInstance* EOSGameInstance = GetGameInstance<UEOSGameInstance>();

	if (CoopGameState && EOSGameInstance)
	{
		CoopGameState->SetSessionName(EOSGameInstance->GetCurrentLobbyName());
	}



}
