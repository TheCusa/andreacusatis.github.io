// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerEntry.h"
#include "CoopGame/Core/CoopPlayerState.h"
#include "Components/TextBlock.h"

void ULobbyPlayerEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	ACoopPlayerState* PlayerState = Cast< ACoopPlayerState>(ListItemObject);
	
	if (PlayerState)
	{
		PlayerNameText->SetText(FText::FromString(PlayerState->GetPlayerName()));
	}
}
