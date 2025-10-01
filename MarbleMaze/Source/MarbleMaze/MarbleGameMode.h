// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MarbleGameMode.generated.h"
/**
 * 
 */
class ABall;
class UNiagaraSystem;
class ABoard;
UCLASS()
class MARBLEMAZE_API AMarbleGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	void HandleVictory();
	bool b_HasAlreadyWon = false;
private:
	UPROPERTY()
	ABoard* BoardRef;
	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* NiagaraRef;
	UPROPERTY()
	APlayerController* PC;

	float Delay = 2.f;
	void ReloadLevel();
};
