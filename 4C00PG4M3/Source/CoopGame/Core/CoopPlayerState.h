// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/Character.h"
#include "CoopPlayerState.generated.h"

class UCharacterDefinition;

/**
 *
 */
UCLASS()
class COOPGAME_API ACoopPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	// Funzione per selezionare/deselezionare personaggi (modificata per gestire toggle)
	UFUNCTION(Server, Reliable)
	void Server_IssueCharacterPicked(const UCharacterDefinition* SelectedCharacter);

	// Funzione per ottenere il personaggio attualmente selezionato
	UFUNCTION(BlueprintCallable)
	const UCharacterDefinition* GetCurrentSelectedCharacter() const { return CurrentSelectedCharacter; }

	// Funzioni per il GameplayGameMode (spawn dei personaggi)
	UFUNCTION(BlueprintCallable)
	UCharacterDefinition* GetPickedCharacterDefinition() const { return const_cast<UCharacterDefinition*>(CurrentSelectedCharacter); }

	UFUNCTION(BlueprintCallable)
	UClass* GetPickedCharacterClass() const;

	// Funzione per deselezionare esplicitamente il personaggio corrente
	UFUNCTION(Server, Reliable)
	void Server_DeselectCurrentCharacter();

	// Funzione per impostare direttamente il personaggio (per il restore dopo travel)
	void SetCurrentSelectedCharacter(const UCharacterDefinition* Character);

private:
	// Implementazioni delle funzioni RPC
	void Server_IssueCharacterPicked_Implementation(const UCharacterDefinition* SelectedCharacter);
	void Server_DeselectCurrentCharacter_Implementation();

	// Riferimento al personaggio attualmente selezionato (replicato)
	UPROPERTY(Replicated)
	const UCharacterDefinition* CurrentSelectedCharacter;

protected:
	// Override per la replicazione
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	// Override per preservare i dati durante il seamless travel
	virtual void CopyProperties(APlayerState* PlayerState) override;
};