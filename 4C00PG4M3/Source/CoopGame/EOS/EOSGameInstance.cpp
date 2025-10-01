// Fill out your copyright notice in the Description page of Project Settings.


#include "EOSGameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Online/OnlineSessionNames.h"

void UEOSGameInstance::JoinLobbyBySearchResultIndex(int index)
{
	if (!sessionPtr || !LoggedInUserId.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot join session: User not logged in"));
		return;
	}

	if (index < 0 || index >= sessionSearch->SearchResults.Num())
	{
		return;
	}

	FOnlineSessionSearchResult& searchResult = sessionSearch->SearchResults[index];
	
	if (sessionPtr)
	{
		sessionPtr->JoinSession(0, GetCurrentLobbyName(), searchResult);
	}
}

void UEOSGameInstance::Login()
{
	if (identityPtr)
	{
		if (identityPtr->GetLoginStatus(0) == ELoginStatus::LoggedIn)
		{
			UE_LOG(LogTemp, Warning, TEXT("User already logged in"));
			OnLoginSuccess.Broadcast();
			return;
		}

		FOnlineAccountCredentials onlineAccountCredentials;
		onlineAccountCredentials.Type = "AccountPortal";
		onlineAccountCredentials.Id = "";
		onlineAccountCredentials.Token = "";

		identityPtr->Login(0, onlineAccountCredentials);
		
	}
}

void UEOSGameInstance::CreateSession(const FName& SessionName)
{
	if (!sessionPtr || !LoggedInUserId.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot create session: User not logged in"));
		return;
	}

	if (sessionPtr)
	{
		FOnlineSessionSettings sessionSettings;
		sessionSettings.bAllowInvites = true;
		sessionSettings.bIsDedicated = false;
		sessionSettings.bIsLANMatch = false;
		sessionSettings.bShouldAdvertise = true;
		sessionSettings.bUseLobbiesIfAvailable = true;
		sessionSettings.bUsesPresence = true;
		sessionSettings.bAllowJoinInProgress = true;
		sessionSettings.bAllowJoinViaPresence = true;
		sessionSettings.NumPublicConnections = 10;

		sessionSettings.Set(SessionNameKey, SessionName.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		CurrentLobbyName = SessionName;
		sessionPtr->CreateSession(*LoggedInUserId, SessionName, sessionSettings);
	}
}

FString UEOSGameInstance::GetNickname()
{
	if (identityPtr.IsValid())
	{
		TSharedPtr<const FUniqueNetId> UserId = identityPtr->GetUniquePlayerId(0);
		if (UserId.IsValid())
		{
			return identityPtr->GetPlayerNickname(*UserId);
		}
		else
		{
			return TEXT("Unknown");
		}
	}
	else
	{
		return TEXT("Unknown");
	}
}

void UEOSGameInstance::FindSession()
{
	if (!sessionPtr || !LoggedInUserId.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot find sessions: User not logged in"));
		return;
	}
	if (sessionPtr)
	{
		sessionSearch = MakeShareable(new FOnlineSessionSearch);
		sessionSearch->bIsLanQuery = false;
		sessionSearch->MaxSearchResults = 100;
		sessionSearch->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
		sessionPtr->FindSessions(0, sessionSearch.ToSharedRef());

	}
}

FString UEOSGameInstance::GetSessionName(const FOnlineSessionSearchResult& searchResult) const
{
	FString outVal = {"Name None"};
	searchResult.Session.SessionSettings.Get(SessionNameKey,outVal);
	return outVal;
}

void UEOSGameInstance::LoadGameLevel()
{

	LoadLevelAndListen(GameLevel);
}

void UEOSGameInstance::Init()
{
	Super::Init();
	onlineSubsystem = IOnlineSubsystem::Get();
	identityPtr = onlineSubsystem->GetIdentityInterface();
	identityPtr->OnLoginCompleteDelegates->AddUObject(this, &UEOSGameInstance::LoginCompleted);
	
	sessionPtr = onlineSubsystem->GetSessionInterface();
	sessionPtr->OnCreateSessionCompleteDelegates.AddUObject(this, &UEOSGameInstance::CreateSessionCompleted);
	sessionPtr->OnFindSessionsCompleteDelegates.AddUObject(this, &UEOSGameInstance::FindSessionCompleted);
	sessionPtr->OnJoinSessionCompleteDelegates.AddUObject(this, &UEOSGameInstance::JoinSessionCompleted);
}

void UEOSGameInstance::LoginCompleted(int numOfPlayers, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Login successful for user: %s"), *UserId.ToString());

		LoggedInUserId = UserId.AsShared();

		FString nickname = GetNickname();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, nickname);

		OnLoginSuccess.Broadcast();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Logged failed: %s"), *Error)
		LoggedInUserId.Reset();
	}
}

void UEOSGameInstance::CreateSessionCompleted(FName name, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Session created successfully"));		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Session creation failed"));
	}
	LoadLevelAndListen(LobbyLevel);
}

void UEOSGameInstance::FindSessionCompleted(bool bWasSuccessful)
{
	if (bWasSuccessful && sessionSearch->SearchResults.Num() > 0)
	{
		for (const FOnlineSessionSearchResult& lobbyFound : sessionSearch->SearchResults)
		{
			FString LobbyName = GetSessionName(lobbyFound);
			UE_LOG(LogTemp, Warning, TEXT("Found session with id: %s"), *LobbyName);
		}
		SearchCompleted.Broadcast(sessionSearch->SearchResults);
	}
}

void UEOSGameInstance::JoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		FString TravelUrl;
		sessionPtr->GetResolvedConnectString(SessionName, TravelUrl);
		UE_LOG(LogTemp, Warning, TEXT("Client travel to: %s"), *TravelUrl);
		GetFirstLocalPlayerController(GetWorld())->ClientTravel(TravelUrl, ETravelType::TRAVEL_Absolute);
	}
}

void UEOSGameInstance::LoadLevelAndListen(TSoftObjectPtr<UWorld> LevelToLoad)
{
	if (!LevelToLoad.IsValid())
	{
		LevelToLoad.LoadSynchronous();
	}
	if (LevelToLoad.IsValid())
	{
		const FString LevelPath = FPackageName::ObjectPathToPackageName(LevelToLoad.ToString());
		FString TravelURL = LevelPath + TEXT("?listen?port=7777");
		GetWorld()->ServerTravel(TravelURL);
	}

}
