// Fill out your copyright notice in the Description page of Project Settings.

#include "CoopPlayerState.h"
#include "CoopGame/Core/CoopGameState.h"
#include "CoopGame/Characters/CharacterDefinition.h"
#include "Net/UnrealNetwork.h"

void ACoopPlayerState::Server_IssueCharacterPicked_Implementation(const UCharacterDefinition* SelectedCharacter)
{
	ACoopGameState* CoopGameState = GetWorld()->GetGameState<ACoopGameState>();
	if (!CoopGameState || !SelectedCharacter)
	{
		return;
	}

	// Se stiamo selezionando lo stesso personaggio che abbiamo già, deselezionalo
	if (CurrentSelectedCharacter == SelectedCharacter)
	{
		// Deseleziona il personaggio corrente
		const UCharacterDefinition* PreviousCharacter = CurrentSelectedCharacter;
		CurrentSelectedCharacter = nullptr;
		CoopGameState->UpdateCharacterSelection(nullptr, PreviousCharacter, this);
	}
	else
	{
		// Controlla se il personaggio è già stato selezionato da un altro giocatore
		if (CoopGameState->IsCharacterSelected(SelectedCharacter))
		{
			// Il personaggio è già selezionato da qualcun altro, non fare nulla
			UE_LOG(LogTemp, Warning, TEXT("Character already selected by another player"));
			return;
		}

		// Seleziona il nuovo personaggio
		const UCharacterDefinition* PreviousCharacter = CurrentSelectedCharacter;
		CurrentSelectedCharacter = SelectedCharacter;
		CoopGameState->UpdateCharacterSelection(SelectedCharacter, PreviousCharacter, this);
	}
}

UClass* ACoopPlayerState::GetPickedCharacterClass() const
{
	if (CurrentSelectedCharacter && CurrentSelectedCharacter->CharacterClass)
	{
		return CurrentSelectedCharacter->CharacterClass;
	}
	return nullptr;
}

void ACoopPlayerState::SetCurrentSelectedCharacter(const UCharacterDefinition* Character)
{
	CurrentSelectedCharacter = Character;
}

void ACoopPlayerState::Server_DeselectCurrentCharacter_Implementation()
{
	if (!CurrentSelectedCharacter)
	{
		return;
	}

	ACoopGameState* CoopGameState = GetWorld()->GetGameState<ACoopGameState>();
	if (CoopGameState)
	{
		const UCharacterDefinition* CharacterToDeselect = CurrentSelectedCharacter;
		CurrentSelectedCharacter = nullptr;
		CoopGameState->UpdateCharacterSelection(nullptr, CharacterToDeselect, this);
	}
}

void ACoopPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACoopPlayerState, CurrentSelectedCharacter);
}

void ACoopPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	// Copia il personaggio selezionato durante il seamless travel
	if (ACoopPlayerState* CoopPS = Cast<ACoopPlayerState>(PlayerState))
	{
		CoopPS->CurrentSelectedCharacter = CurrentSelectedCharacter;

		UE_LOG(LogTemp, Warning, TEXT("CopyProperties: Copied character selection '%s' to new PlayerState"),
			CurrentSelectedCharacter ? *CurrentSelectedCharacter->CharacterName.ToString() : TEXT("None"));
	}
}