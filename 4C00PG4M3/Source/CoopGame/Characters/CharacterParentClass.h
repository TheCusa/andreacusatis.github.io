// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameFramework/Character.h"
#include "EnhancedInputSubsystemInterface.h"
#include "CoopGame/Core/PlayerControllers/CoopPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Logging/LogMacros.h"
#include "CharacterParentClass.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UUserWidget;
class UWidgetInteractionComponent;
class UInputAction;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS()
class COOPGAME_API ACharacterParentClass : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* GameplayMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;


public:

	UPROPERTY(EditDefaultsOnly, Category="UI Navigation Input")
	UInputAction* IA_Left;

	UPROPERTY(EditDefaultsOnly, Category="UI Navigation Input")
	UInputAction* IA_Right;

	UPROPERTY(EditDefaultsOnly, Category="Game Interaction")
	UInputAction* IA_CloseWidget;

	UPROPERTY(EditDefaultsOnly, Category="Game Interaction")
	UInputAction* IA_OpenPuzzle1Widget;

	UPROPERTY(EditDefaultsOnly, Category="UI Navigation Input")
	UInputMappingContext* UIMappingContext;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetInteractionComponent* WidgetInteractionComp;

	
	// Sets default values for this character's properties
	ACharacterParentClass();

protected:

	// References to UI Mapping context and Input Actions

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	
	/** Called for interact input */
	void Interact();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Widget BP class reference. Assignable from deatils panel
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> InteractUIWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Navigation Input")
	TObjectPtr<UInputAction> LeftClickAction; 
	
	void OnLeftClickPressed();
	void OnLeftClickReleased();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void ForwardHandleLeft(const FInputActionValue& Value);
	void ForwardHandleRight(const FInputActionValue& Value);
	void ForwardOpenWidget(const FInputActionValue& Value);// , GameUserWidget WidgetName);
	void ForwardCloseWidget(const FInputActionValue& Value);

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// Set the player's most nearby object.
	void SetNearbyInteractableObject(AActor* InteractableObject);
	// Clear the player's nearby object.
	void ClearNearbyInteractableObject(AActor* InteractableObject);

	// Lock player movement component.
	void LockCharacterMovement() const;

	// Unock player movement component 
	void UnlockCharacterMovement() const;

private:
	// Nearby object reference
	AActor* NearbyInteractableObject;

	// Widget istance
	UUserWidget* InteractUIWidget;

	// Local player enanched input subsystem reference
	UEnhancedInputLocalPlayerSubsystem* Subsystem;

	void ShowUIPrompt();
	void HideUIPrompt();
};
