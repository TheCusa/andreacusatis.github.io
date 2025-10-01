// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyWidget.h"
#include "Components/TextBlock.h"
#include "Components/ListView.h"
#include "CoopGame/Core/CoopGameState.h"
#include "CoopGame/Core/CoopPlayerState.h"
#include "CoopGame/EOS/EOSGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"
#include "Components/TileView.h"
#include "CoopGame/Characters/CharacterDefinition.h"
#include "CharacterEntry.h"
#include "Components/Button.h"
#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"

void ULobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();
	GameState = Cast<ACoopGameState>(UGameplayStatics::GetGameState(this));

	if (GameState)
	{
		FName lobbyName = GameState->GetSessionName();
		LobbyNameText->SetText(FText::FromName(lobbyName));
		GameState->OnSessionNameReplicated.AddDynamic(this, &ULobbyWidget::SessionNameReplicated);

		PlayerList->SetListItems(GameState->PlayerArray);

		GetWorld()->GetTimerManager().SetTimer(PlayerListUpdateTimerHandle, this, &ULobbyWidget::RefreshPlayerList, 0.5f, true);

		CharacterList->SetListItems(GameState->GetCharacters());
		CharacterList->OnItemSelectionChanged().AddUObject(this, &ULobbyWidget::PlayerSelectionIssued);

		GameState->OnCharacterSelectionReplicated.AddDynamic(this, &ULobbyWidget::CharacterSelectionReplicated);
	}

	PlayerState = GetOwningPlayerState<ACoopPlayerState>();

	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &ULobbyWidget::LoadGame);
		UpdateStartButtonVisibility();
	}

	// Timer per aggiornare periodicamente il bottone start
	GetWorld()->GetTimerManager().SetTimer(StartButtonUpdateTimerHandle, this, &ULobbyWidget::UpdateStartButtonState, 0.5f, true);
}

void ULobbyWidget::NativeDestruct()
{
	// Pulisci i timer per evitare crash
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(PlayerListUpdateTimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(StartButtonUpdateTimerHandle);
	}

	Super::NativeDestruct();
}

void ULobbyWidget::SessionNameReplicated(const FName& newSessionName)
{
	LobbyNameText->SetText(FText::FromName(newSessionName));
}

void ULobbyWidget::CharacterSelectionReplicated(const UCharacterDefinition* selected, const UCharacterDefinition* deselected)
{
	if (selected != nullptr)
	{
		UCharacterEntry* characterEntry = CharacterList->GetEntryWidgetFromItem<UCharacterEntry>(selected);
		if (characterEntry)
		{
			characterEntry->SetCharacterSelected(true);
		}
	}

	if (deselected != nullptr)
	{
		UCharacterEntry* characterEntry = CharacterList->GetEntryWidgetFromItem<UCharacterEntry>(deselected);
		if (characterEntry)
		{
			characterEntry->SetCharacterSelected(false);
		}
	}

	// Aggiorna lo stato del bottone start
	UpdateStartButtonState();
}

void ULobbyWidget::RefreshPlayerList()
{
	if (GameState)
	{
		PlayerList->SetListItems(GameState->PlayerArray);
		// Aggiorna anche il bottone quando cambiano i giocatori
		UpdateStartButtonState();
	}
}

void ULobbyWidget::PlayerSelectionIssued(UObject* Item)
{
	UCharacterDefinition* SelectedCharacterDefinition = Cast<UCharacterDefinition>(Item);
	if (SelectedCharacterDefinition && PlayerState)
	{
		// La logica di selezione/deselezione è gestita nel PlayerState
		PlayerState->Server_IssueCharacterPicked(SelectedCharacterDefinition);

		// Deseleziona l'item nella UI per evitare problemi di stato
		CharacterList->ClearSelection();
	}
}

void ULobbyWidget::LoadGame()
{
	// Solo l'host può avviare il gioco
	if (!IsHost())
	{
		UE_LOG(LogTemp, Warning, TEXT("Only host can start the game"));
		return;
	}

	// Verifica che il gioco possa essere avviato
	if (!GameState || !GameState->CanStartGame())
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot start game: not all players have selected characters"));
		return;
	}

	UEOSGameInstance* GameInstance = GetGameInstance<UEOSGameInstance>();
	if (GameInstance)
	{
		GameInstance->LoadGameLevel();
	}
}

bool ULobbyWidget::IsHost() const
{
	// Controlla se questo client è l'host verificando la presenza del GameMode
	UWorld* World = GetWorld();
	if (World)
	{
		AGameModeBase* GameMode = World->GetAuthGameMode();
		return GameMode != nullptr; // Solo l'host ha un GameMode valido
	}
	return false;
}

void ULobbyWidget::UpdateStartButtonVisibility()
{
	if (StartButton)
	{
		// Mostra il bottone solo all'host
		StartButton->SetVisibility(IsHost() ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void ULobbyWidget::UpdateStartButtonState()
{
	if (StartButton && IsHost())
	{
		// Abilita il bottone solo se il gioco può essere avviato
		bool bCanStart = GameState ? GameState->CanStartGame() : false;
		StartButton->SetIsEnabled(bCanStart);

		// Aggiorna il tooltip per dare feedback all'utente
		if (bCanStart)
		{
			StartButton->SetToolTipText(FText::FromString(TEXT("Start Game")));
		}
		else
		{
			int32 PlayersReady = GameState ? GameState->GetSelectedCharactersCount() : 0;
			FString TooltipText = FString::Printf(TEXT("Players ready: %d/2"), PlayersReady);
			StartButton->SetToolTipText(FText::FromString(TooltipText));
		}
	}
}