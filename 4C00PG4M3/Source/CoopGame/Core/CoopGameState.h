// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CoopGameState.generated.h"

class AAgentPlayerController;
class AHackerPlayerController;
class UCharacterDefinition;
class FLifetimeProperty;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSessionNameReplicated, const FName&, name);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterSelectionReplicated, const UCharacterDefinition*, selected, const UCharacterDefinition*, deselected);

/**
 *
 */

UENUM(BlueprintType)
enum class EPuzzleType : uint8
{
	None        UMETA(DisplayName="None"),
	RoomTemperatureIQ UMETA(DisplayName="RoomTemperatureIQ Puzzle"),
	LaserRun UMETA(DisplayName="LaserRun Puzzle"),
	SafePath  UMETA(DisplayName="SafePath Puzzle")
};

UCLASS()
class COOPGAME_API ACoopGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	FOnSessionNameReplicated OnSessionNameReplicated;
	FOnCharacterSelectionReplicated OnCharacterSelectionReplicated;
	FORCEINLINE FName GetSessionName() const { return SessionName; };
	void SetSessionName(const FName& newSessionName);
	const TArray<UCharacterDefinition*>& GetCharacters() const;
	
	bool IsCharacterSelected(const UCharacterDefinition* characterToCheck) const;
	UPROPERTY(Replicated)
	TObjectPtr<AHackerPlayerController> HackerPlayerController;
	UPROPERTY(Replicated)
	TObjectPtr<AAgentPlayerController> AgentPlayerController;
	UPROPERTY(Replicated)
	TArray<int8> CodePuzzleSolution = {1, 2, 3, 4};
	UPROPERTY(ReplicatedUsing = OnRep_IsAlarmActive)
	bool IsAlarmActive = false;
	UPROPERTY(Replicated)
	int32 GameTimer;
	// Funzione modificata per includere il PlayerState
	void UpdateCharacterSelection(const UCharacterDefinition* selected, const UCharacterDefinition* deselected, class APlayerState* Player);

	// Nuove funzioni per la gestione avanzata
	UFUNCTION(BlueprintCallable)
	bool CanStartGame() const;

	UFUNCTION(BlueprintCallable)
	int32 GetSelectedCharactersCount() const { return SelectedCharacters.Num(); }

	// Funzione per ottenere il personaggio selezionato da un giocatore specifico
	const UCharacterDefinition* GetCharacterSelectedByPlayer(class APlayerState* Player) const;

	// Funzione per ripristinare la selezione dopo il travel
	UFUNCTION(BlueprintCallable)
	void RestorePlayerCharacterSelection(class APlayerState* Player);

	UFUNCTION()
	void SetAlarm(bool Value);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAlarmChanged, bool, bNewState);

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnAlarmChanged OnAlarmChanged;

private:
	UPROPERTY(ReplicatedUsing = OnRep_SessionName)
	FName SessionName;

	UFUNCTION()
	void OnRep_SessionName();

	UFUNCTION()
	void OnRep_IsAlarmActive();

	UPROPERTY(EditDefaultsOnly)
	TArray<UCharacterDefinition*> Characters;

	TArray<const UCharacterDefinition*> SelectedCharacters;

	// Mappa per tracciare quale giocatore ha selezionato quale personaggio
	UPROPERTY()
	TMap<class APlayerState*, const UCharacterDefinition*> PlayerCharacterSelections;

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_UpdatedCharacterSelection(const UCharacterDefinition* selected, const UCharacterDefinition* deselected);

	// Numero massimo di giocatori per il gioco
	static constexpr int32 MaxPlayers = 2;

protected:
	/** Returns properties that are replicated for the lifetime of the actor channel */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};