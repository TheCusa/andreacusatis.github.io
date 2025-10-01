// Fill out your copyright notice in the Description page of Project Settings.

#include "CoopGameState.h"
#include "Net/UnrealNetwork.h"
#include "CoopGame/Characters/CharacterDefinition.h"
#include "CoopGame/Core/CoopPlayerState.h"
#include "GameFramework/PlayerState.h"

void ACoopGameState::SetSessionName(const FName& newSessionName)
{
	SessionName = newSessionName;
}

const TArray<class UCharacterDefinition*>& ACoopGameState::GetCharacters() const
{
	return Characters;
}

bool ACoopGameState::IsCharacterSelected(const UCharacterDefinition* characterToCheck) const
{
	return SelectedCharacters.Contains(characterToCheck);
}

void ACoopGameState::UpdateCharacterSelection(const UCharacterDefinition* selected, const UCharacterDefinition* deselected, APlayerState* Player)
{
	if (!HasAuthority()) return;

	// Se c'� una deselezione, rimuovila
	if (deselected != nullptr)
	{
		PlayerCharacterSelections.Remove(Player);
		SelectedCharacters.Remove(deselected);
	}

	// Se c'� una selezione, aggiungila
	if (selected != nullptr)
	{
		// Prima rimuovi eventuali selezioni precedenti del giocatore
		const UCharacterDefinition* const* PreviousSelection = PlayerCharacterSelections.Find(Player);
		if (PreviousSelection)
		{
			SelectedCharacters.Remove(*PreviousSelection);
		}

		PlayerCharacterSelections.Add(Player, selected);
		SelectedCharacters.Add(selected);
	}

	NetMulticast_UpdatedCharacterSelection(selected, deselected);
}

bool ACoopGameState::CanStartGame() const
{
	// Controlla se ci sono abbastanza giocatori e se ognuno ha selezionato un personaggio
	return PlayerArray.Num() >= MaxPlayers && PlayerCharacterSelections.Num() >= MaxPlayers;
}

const UCharacterDefinition* ACoopGameState::GetCharacterSelectedByPlayer(APlayerState* Player) const
{
	const UCharacterDefinition* const* FoundCharacter = PlayerCharacterSelections.Find(Player);
	return FoundCharacter ? *FoundCharacter : nullptr;
}

void ACoopGameState::RestorePlayerCharacterSelection(APlayerState* Player)
{
	// Trova la selezione del personaggio per questo giocatore
	const UCharacterDefinition* SelectedCharacter = GetCharacterSelectedByPlayer(Player);

	if (SelectedCharacter)
	{
		// Ripristina la selezione nel PlayerState se non � gi� impostata
		if (ACoopPlayerState* CoopPS = Cast<ACoopPlayerState>(Player))
		{
			if (!CoopPS->GetCurrentSelectedCharacter())
			{
				// Usa una funzione diretta invece di RPC per evitare problemi di autorit�
				CoopPS->SetCurrentSelectedCharacter(SelectedCharacter);
				UE_LOG(LogTemp, Warning, TEXT("RestorePlayerCharacterSelection: Restored character '%s' for player %s"),
					*SelectedCharacter->CharacterName.ToString(), *Player->GetPlayerName());
			}
		}
	}
}

void ACoopGameState::SetAlarm(bool Value)
{
	IsAlarmActive = Value;
	OnAlarmChanged.Broadcast(IsAlarmActive);
	UE_LOG( LogTemp, Warning, TEXT("Alarm State changed to: %s"), IsAlarmActive ? TEXT("ACTIVE") : TEXT("INACTIVE"));
}

void ACoopGameState::OnRep_IsAlarmActive()
{
	OnAlarmChanged.Broadcast(IsAlarmActive);
}

void ACoopGameState::OnRep_SessionName()
{
	OnSessionNameReplicated.Broadcast(SessionName);
}

void ACoopGameState::NetMulticast_UpdatedCharacterSelection_Implementation(const UCharacterDefinition* selected, const UCharacterDefinition* deselected)
{
	OnCharacterSelectionReplicated.Broadcast(selected, deselected);
}

void ACoopGameState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(ACoopGameState, SessionName, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME(ACoopGameState, CodePuzzleSolution);
	DOREPLIFETIME(ACoopGameState, HackerPlayerController);
	DOREPLIFETIME(ACoopGameState, AgentPlayerController);
	DOREPLIFETIME(ACoopGameState, IsAlarmActive);
	DOREPLIFETIME(ACoopGameState, GameTimer);
}
