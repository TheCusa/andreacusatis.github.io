// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CoopGame/SoundPuzzle/SoundDataAsset.h"
#include "MelodiesDataTable.generated.h"

/**
 * 
 */
USTRUCT()
struct FMelodyRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly)
    int8 MelodyID = 0;

    UPROPERTY(EditDefaultsOnly)
    TArray<Notes> Notes;
};

UCLASS()
class COOPGAME_API UMelodiesDataTable : public UDataTable
{
	GENERATED_BODY()
	
};
