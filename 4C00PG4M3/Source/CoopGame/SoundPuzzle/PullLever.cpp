#include "PullLever.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "CoopGame/Characters/Agent/AgentCharacter.h"
#include "CoopGame/Core/PlayerControllers/AgentPlayerController.h"
#include "CoopGame/SoundPuzzle/Piano.h"
#include "Curves/CurveFloat.h"

#include "CoopGame/Core/Puzzle/Utils.h"

// Sets default values
APullLever::APullLever()
{
	// Set this actor to call Tick() every frame only when needed
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false; 

	InteractionCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider Component"));

	// Set box collision component as root component
	RootComponent = InteractionCollider;

	// Create the mesh component and attach it to root component
	StaticBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	StaticBaseMesh->SetupAttachment(RootComponent);
	
	MovableLeverMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Movable Mesh Component"));
	MovableLeverMesh->SetupAttachment(StaticBaseMesh);

	// Create timeline component
	LeverTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("LeverTimeline"));
	

	// Initialize animation properties
	LeverPullAngle = 45.0f;
	bIsAnimating = false;

}

void APullLever::ExecuteAction()
{
	if (bIsAnimating)
	{
		return;
	}

	// Start lever animation
	if (LeverTimeline && LeverCurve)
	{
		bIsAnimating = true;
		SetActorTickEnabled(true);
		LeverTimeline->PlayFromStart();
	}

	if (AgentPlayerControllerRef && PianoReference)
	{
		const TArray<int8> PuzzleSolution = PianoReference->GetPuzzleSolution();
		AgentPlayerControllerRef->SendArrayCode(PuzzleSolution, GameUserWidget::None);
		
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("SendPuzzleSolution Piano"));
		}
	}
}

void APullLever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Tick the timeline component only when animating
	if (LeverTimeline && bIsAnimating)
	{
		LeverTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, nullptr);
	}
}

// Called when the game starts or when spawned
void APullLever::BeginPlay()
{
	Super::BeginPlay();

	InteractionCollider->OnComponentBeginOverlap.AddDynamic(this, &APullLever::OnInteractionBoxOverlapBegin);
	InteractionCollider->OnComponentEndOverlap.AddDynamic(this, &APullLever::OnInteractionBoxOverlapEnd);

	GameStateRef = GetWorld()->GetGameState<ACoopGameState>();
	if (GameStateRef)
	{
		AgentPlayerControllerRef = GameStateRef->AgentPlayerController;
	}

	if (MovableLeverMesh)
	{
		// Store initial rotation
		InitialRotation = MovableLeverMesh->GetRelativeRotation();
		TargetRotation = InitialRotation + FRotator(0.0f, LeverPullAngle, 0.0f);
	}

	if (LeverCurve && LeverTimeline)
	{
		// Create timeline callback delegates
		FOnTimelineFloat TimelineCallback;
		FOnTimelineEventStatic  TimelineFinishedCallback;

		// Bind the callback functions
		TimelineCallback.BindUFunction(this, FName(TEXT("TimelineUpdate")));
		TimelineFinishedCallback.BindUFunction(this, FName(TEXT("TimelineFinished")));

		// Configure timeline
		LeverTimeline->AddInterpFloat(LeverCurve, TimelineCallback);
		LeverTimeline->SetTimelineFinishedFunc(TimelineFinishedCallback);
		LeverTimeline->SetLooping(false);
	}
}

void APullLever::TimelineUpdate(const float Alpha) const
{
	if (MovableLeverMesh)
	{
		// Interpolate between initial and target rotation based on Alpha (0-1)
		const FRotator CurrentRotation = FMath::Lerp(InitialRotation, TargetRotation, Alpha);
		MovableLeverMesh->SetRelativeRotation(CurrentRotation);
	}
}

void APullLever::TimelineFinished()
{
	if (LeverTimeline)
	{
		const float CurrentPosition = LeverTimeline->GetPlaybackPosition();

		// Check if lever is down
		if (FMath::IsNearlyEqual(CurrentPosition, LeverTimeline->GetTimelineLength(), 0.01f))
		{
			// Delay
			FTimerHandle ReverseTimer;
			GetWorld()->GetTimerManager().SetTimer(ReverseTimer, [this]()
				{
					if (LeverTimeline)
					{
						LeverTimeline->Reverse();
					}
				}, 0.3f, false);
		}
		
		// Check if lever is up
		else if (FMath::IsNearlyEqual(CurrentPosition, 0.0f, 0.01f))
		{
			bIsAnimating = false;
			SetActorTickEnabled(false);
		}
	}
}

void APullLever::OnInteractionBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || (OtherActor == this))
	{
		return;
	}

	AAgentCharacter* OverlapCharacter = Cast<AAgentCharacter>(OtherActor);
	if (OverlapCharacter)
	{
		// Check if OverlapCharacter is the local player. It's needed to execute this event only in local
		if (!OverlapCharacter->IsLocallyControlled())
		{
			return;
		}
		//UE_LOG(LogTemp, Warning, TEXT("BEGIN OVERLAP"));
		OverlapCharacter->SetNearbyInteractableObject(this);
	}
}

void APullLever::OnInteractionBoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (!OtherActor || (OtherActor == this))
	{
		return;
	}

	AAgentCharacter* OverlapCharacter = Cast<AAgentCharacter>(OtherActor);
	if (OverlapCharacter)
	{
		// Check if OverlapCharacter is the local player. It's needed to execute this event only in local
		if (!OverlapCharacter->IsLocallyControlled())
		{
			return;
		}
		//UE_LOG(LogTemp, Warning, TEXT("END OVERLAP"));
		OverlapCharacter->ClearNearbyInteractableObject(this);
	}
}
