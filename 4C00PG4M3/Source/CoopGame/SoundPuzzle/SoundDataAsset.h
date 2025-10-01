// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SoundDataAsset.generated.h"

/**
 * 
 */
UENUM()
enum  class Notes : int8
{
	None = -1,
	DO_1,
	REb_1,
	RE_1,
	MIb_1,
	MI_1,
	FA_1,
	SOLb_1,
	SOL_1,
	LAb_1,
	LA_1,
	SIb_1,
	SI_1,

	DO_2,
	REb_2,
	RE_2,
	MIb_2,
	MI_2,
	FA_2,
	SOLb_2,
	SOL_2,
	LAb_2,
	LA_2,
	SIb_2,
	SI_2,

};
UCLASS()
class COOPGAME_API USoundDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
	TMap<Notes, USoundBase*> SoundMap;

	
public:
	USoundBase* GetKeySound(const Notes) const;
};
