// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"


class UCharacterDefinition;

/**
 * 
 */
UCLASS()
class COOPGAME_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeDestruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LobbyNameText;

	UPROPERTY(meta = (BindWidget))
	class UListView* PlayerList;

	UPROPERTY(meta = (BindWidget))
	class UTileView* CharacterList;

	UPROPERTY(meta = (BindWidget))
	class UButton* StartButton;

	UPROPERTY()
	class ACoopGameState* GameState;

	UPROPERTY()
	class ACoopPlayerState* PlayerState;

	// Timer handles
	FTimerHandle PlayerListUpdateTimerHandle;
	FTimerHandle StartButtonUpdateTimerHandle;

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void SessionNameReplicated(const FName& newSessionName);

	UFUNCTION()
	void CharacterSelectionReplicated(const UCharacterDefinition* selected, const UCharacterDefinition* deselected);

	UFUNCTION()
	void LoadGame();

	// Funzioni per la gestione del bottone Start
	bool IsHost() const;
	void UpdateStartButtonVisibility();
	void UpdateStartButtonState();

	void RefreshPlayerList();
	void PlayerSelectionIssued(UObject* Item);
};