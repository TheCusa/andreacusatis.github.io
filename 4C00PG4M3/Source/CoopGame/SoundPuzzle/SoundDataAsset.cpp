// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundDataAsset.h"

USoundBase* USoundDataAsset::GetKeySound(const Notes NoteID) const
{
	if (!SoundMap.IsEmpty() && SoundMap.Contains(NoteID))
	{
		return SoundMap[NoteID];
	}
	return nullptr;
}