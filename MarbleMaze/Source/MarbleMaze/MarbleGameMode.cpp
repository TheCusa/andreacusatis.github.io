// Fill out your copyright notice in the Description page of Project Settings.


#include "MarbleGameMode.h"
#include "Ball.h"
#include "Kismet/GamePlayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Board.h"

void AMarbleGameMode::HandleVictory()
{
	BoardRef = Cast<ABoard>(UGameplayStatics::GetActorOfClass(this, ABoard::StaticClass()));
	PC = UGameplayStatics::GetPlayerController(this, 0);

	if (BoardRef)
	{
		BoardRef->DisableInput(PC);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, NiagaraRef, BoardRef->BoxCollisionLocation);
		FTimerHandle TimerHandler;
		GetWorld()->GetTimerManager().SetTimer(TimerHandler, this, &AMarbleGameMode::ReloadLevel, Delay, false);
	}
}
void AMarbleGameMode::ReloadLevel()
{
		FString CurrentLevel;
		CurrentLevel = UGameplayStatics::GetCurrentLevelName(this);
		UGameplayStatics::OpenLevel(this, *CurrentLevel);
}