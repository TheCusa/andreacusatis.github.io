// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginMenu.h"
#include "CoopGame/EOS/EOSGameInstance.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/ScrollBox.h"
#include "OnlineSessionSettings.h"
#include "LobbyEntry.h"


void ULoginMenu::NativeConstruct()
{
	Super::NativeConstruct();
	GameInstance = GetGameInstance<UEOSGameInstance>();

	LoginBtn->OnClicked.AddDynamic(this, &ULoginMenu::LoginBtnClicked);

	CreateSessionBtn->OnClicked.AddDynamic(this, &ULoginMenu::CreateSessionBtnClicked);
	CreateSessionBtn->SetIsEnabled(false);

	FindSessionBtn->OnClicked.AddDynamic(this, &ULoginMenu::FindSessionBtnClicked);
	FindSessionBtn->SetIsEnabled(false);

	SessionNameText->OnTextChanged.AddDynamic(this, &ULoginMenu::SessionNameChanged);

	JoinLobbyBtn->OnClicked.AddDynamic(this, &ULoginMenu::JoinLobbyBtnClicked);
	JoinLobbyBtn->SetIsEnabled(false);

	GameInstance->SearchCompleted.AddUObject(this, &ULoginMenu::SessionSearchCompleted);

	
	if (GameInstance)
	{
		GameInstance->OnLoginSuccess.AddUObject(this, &ULoginMenu::HandleLoginSuccess);
	}
}

void ULoginMenu::HandleLoginSuccess()
{
	UE_LOG(LogTemp, Warning, TEXT("LoginMenu: Login successful! Enabling buttons."));

	SessionNameChanged(SessionNameText->GetText());

	FindSessionBtn->SetIsEnabled(true);

	//LoginBtn->SetIsEnabled(false);
}

void ULoginMenu::LoginBtnClicked()
{
	if (GameInstance)
	{
		GameInstance->Login();
	}
}

void ULoginMenu::CreateSessionBtnClicked()
{
	if (GameInstance)
	{
		GameInstance->CreateSession(FName(SessionNameText->GetText().ToString()));
	}
}

void ULoginMenu::FindSessionBtnClicked()
{
	if (GameInstance)
	{
		GameInstance->FindSession();
	}
}

void ULoginMenu::SessionNameChanged(const FText& text)
{
	CreateSessionBtn->SetIsEnabled(!text.IsEmpty());
}

void ULoginMenu::LobbyEntrySelected(int lobbyEntryIndex)
{
	SelectedLobbyEntryIndex = lobbyEntryIndex;
	if (SelectedLobbyEntryIndex != -1)
	{
		JoinLobbyBtn->SetIsEnabled(true);
	}
}

void ULoginMenu::JoinLobbyBtnClicked()
{
	if (GameInstance)
	{
		GameInstance->JoinLobbyBySearchResultIndex(SelectedLobbyEntryIndex);
	}
}

void ULoginMenu::SessionSearchCompleted(const TArray<FOnlineSessionSearchResult>& searchResults)
{
	LobbyListScrollBox->ClearChildren();
	int index = 0;
	for (const FOnlineSessionSearchResult& SearchResult : searchResults)
	{
		FString sessionName = GameInstance->GetSessionName(SearchResult);
		ULobbyEntry* LobbyEntry = CreateWidget<ULobbyEntry>(LobbyListScrollBox, LobbyEntryClass);
		LobbyEntry->InitLobbyEntry(FName(sessionName), index);		
		LobbyListScrollBox->AddChild(LobbyEntry);
		LobbyEntry->OnLobbyEntrySelected.AddDynamic(this, &ULoginMenu::LobbyEntrySelected);
		++index;
	}
}
