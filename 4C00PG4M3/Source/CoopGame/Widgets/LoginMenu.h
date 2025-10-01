// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginMenu.generated.h"

/**
 * 
 */
UCLASS()
class COOPGAME_API ULoginMenu : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	class UEOSGameInstance* GameInstance;
	
	UFUNCTION()
	void HandleLoginSuccess();

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* LoginBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* CreateSessionBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* FindSessionBtn;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* LobbyListScrollBox;

	UPROPERTY(meta = (BindWidget))
	class UEditableText* SessionNameText;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ULobbyEntry> LobbyEntryClass;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* JoinLobbyBtn;


	UFUNCTION()
	void LoginBtnClicked();

	UFUNCTION()
	void CreateSessionBtnClicked();

	UFUNCTION()
	void FindSessionBtnClicked();
	
	UFUNCTION()
	void SessionNameChanged(const FText& text);
	
	UFUNCTION()
	void LobbyEntrySelected(int lobbyEntryIndex);
	
	UFUNCTION()
	void JoinLobbyBtnClicked();

	void SessionSearchCompleted(const TArray<FOnlineSessionSearchResult>& searchResults);
	
	int SelectedLobbyEntryIndex = -1;	
};
