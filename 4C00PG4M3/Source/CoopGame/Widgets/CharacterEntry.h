// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "CharacterEntry.generated.h"

/**
 * 
 */
UCLASS()
class COOPGAME_API UCharacterEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	void SetCharacterSelected(bool bIsSelected);

protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject);

private:
	UPROPERTY(meta = (BindWidget))
	class UImage* IconImage;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NameText;

	UPROPERTY(EditDefaultsOnly)
	FName IconMaterialParamName{ "Icon" };
	
	UPROPERTY(EditDefaultsOnly)
	FName SaturationMaterialParamName{ "Saturation" };
	
};
