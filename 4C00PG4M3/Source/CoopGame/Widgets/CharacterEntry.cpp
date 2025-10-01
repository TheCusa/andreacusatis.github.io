// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterEntry.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "CoopGame/Characters/CharacterDefinition.h"

void UCharacterEntry::SetCharacterSelected(bool bIsSelected)
{
	// Non disabilitare completamente l'entry per permettere la deselezione
	// Solo cambia l'aspetto visivo
	IconImage->GetDynamicMaterial()->SetScalarParameterValue(SaturationMaterialParamName, bIsSelected ? 0.3f : 1.0f);

	// Opzionale: cambia il colore del testo per indicare la selezione
	if (NameText)
	{
		FLinearColor TextColor = bIsSelected ? FLinearColor(0.5f, 0.5f, 0.5f, 1.0f) : FLinearColor::White;
		NameText->SetColorAndOpacity(FSlateColor(TextColor));
	}
}

void UCharacterEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	UCharacterDefinition* characterDefinition = Cast<UCharacterDefinition>(ListItemObject);
	if (characterDefinition)
	{
		NameText->SetText(FText::FromName(characterDefinition->CharacterName));
		IconImage->GetDynamicMaterial()->SetTextureParameterValue(IconMaterialParamName, characterDefinition->Icon);
	}
}