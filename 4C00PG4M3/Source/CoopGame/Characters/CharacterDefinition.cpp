// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterDefinition.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

USkeletalMesh* UCharacterDefinition::GetMesh() const
{
	if (CharacterClass)
	{
		ACharacter* characterCDO = Cast<ACharacter>(CharacterClass->GetDefaultObject());
		if (characterCDO)
		{
			return characterCDO->GetMesh()->GetSkeletalMeshAsset();
		}
	}
	return nullptr;
}