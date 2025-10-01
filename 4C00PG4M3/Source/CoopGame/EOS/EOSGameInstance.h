// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EOSGameInstance.generated.h"

namespace EOnJoinSessionCompleteResult
{
	enum Type : int;
}

DECLARE_MULTICAST_DELEGATE(FOnLoginSuccess);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSessionSearchCompleted, const TArray<FOnlineSessionSearchResult>&/*SearchResults*/);
/**
 * 
 */
UCLASS()
class COOPGAME_API UEOSGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	void JoinLobbyBySearchResultIndex(int index);
	FOnLoginSuccess OnLoginSuccess;
	FOnSessionSearchCompleted SearchCompleted;

	UFUNCTION(BlueprintCallable, Category = "EOS")
	void Login();

	UFUNCTION(BlueprintCallable, Category = "EOS")
	void CreateSession(const FName& SessionName);

	UFUNCTION(BlueprintCallable, Category = "EOS")
	FString GetNickname();
	
	UFUNCTION(BlueprintCallable, Category = "EOS")
	void FindSession();

	FORCEINLINE FName GetSessionName() const { return SessionNameKey; }
	FString GetSessionName(const FOnlineSessionSearchResult& searchResult) const;
	FORCEINLINE FName GetCurrentLobbyName() const { return CurrentLobbyName; };
	void LoadGameLevel();

protected:
	virtual void Init() override;

private:
	TSharedPtr<const FUniqueNetId> LoggedInUserId;
	class IOnlineSubsystem* onlineSubsystem;
	TSharedPtr<class IOnlineIdentity, ESPMode::ThreadSafe> identityPtr;
	TSharedPtr<class IOnlineSession, ESPMode::ThreadSafe> sessionPtr;
	void LoginCompleted(int numOfPlayers, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);
	void CreateSessionCompleted(FName name, bool bWasSuccessful);
	void FindSessionCompleted(bool bWasSuccessful);
	void JoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> GameLevel;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> LobbyLevel;

	TSharedPtr<class FOnlineSessionSearch> sessionSearch;

	const FName SessionNameKey{ "SessionNameKey" };

	void LoadLevelAndListen(TSoftObjectPtr<UWorld> LevelToLoad);

	FName CurrentLobbyName;
};
