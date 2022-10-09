#include "Components/InteractionComponent.h"

#include "Core/InteractionInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Recalculate TargetActor every frame ! Can optimisation with timer
	PrepareTargetActor();
}

void UInteractionComponent::PrepareTargetActor()
{
	switch (InteractionType)
	{
	case EInteractionType::Trace:
		return SpawnTrace();

	case EInteractionType::Sphere:
		return SpawnSphere();
	}
}

void UInteractionComponent::SpawnTrace()
{
	if (!GetWorld()) return;
	
	const auto CameraManagerRaw = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	const auto CameraForwardVector = CameraManagerRaw->GetActorForwardVector();
	const auto StartLocation = CameraManagerRaw->GetCameraLocation();
	const auto EndLocation = StartLocation + CameraForwardVector * Distance;
	const auto ActorToIgnore = TArray<AActor*>{GetOwner()};
	const auto DrawType = DrawDebug ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;

	FHitResult HitResult;
	const auto IsHit = UKismetSystemLibrary::LineTraceSingle(this, StartLocation, EndLocation,
	                                                         UEngineTypes::ConvertToTraceType(ECC_Camera), false,
	                                                         ActorToIgnore, DrawType, HitResult, true);

	const auto NewTargetActor = (IsHit && HitResult.GetActor()->Implements<UInteractionInterface>())
		                            ? HitResult.GetActor()
		                            : nullptr;

	if (NewTargetActor == TargetActor) return;
	TargetActor = NewTargetActor;

	TargetIsChanged.Broadcast(TargetActor);
}

void UInteractionComponent::SpawnSphere()
{
	const auto SelfOwner = GetOwner();

	static const auto FilterInteraction = [](const FHitResult Item)
	{
		return Item.bBlockingHit && Item.GetActor()->Implements<UInteractionInterface>();
	};
	static const auto SortInteractionByDistance = [SelfOwner](const FHitResult& A, const FHitResult& B)
	{
		const auto DistanceA = SelfOwner->GetDistanceTo(A.GetActor());
		const auto DistanceB = SelfOwner->GetDistanceTo(B.GetActor());

		return DistanceA < DistanceB;
	};

	const auto StartLocation = SelfOwner->GetActorLocation();
	const auto EndLocation = SelfOwner->GetActorLocation();
	const auto ActorToIgnore = TArray<AActor*>{SelfOwner};
	const auto DrawType = DrawDebug ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;

	TArray<FHitResult> HitResults;
	const bool IsHit = UKismetSystemLibrary::SphereTraceMulti(this, StartLocation, EndLocation, Distance,
	                                                          UEngineTypes::ConvertToTraceType(ECC_Camera), false,
	                                                          ActorToIgnore, DrawType, HitResults, true);

	AActor* NewTargetActor = nullptr;
	auto InteractionHitResults = HitResults.FilterByPredicate(FilterInteraction);

	if (IsHit && InteractionHitResults.Num() > 0)
	{
		InteractionHitResults.Sort(SortInteractionByDistance);
		NewTargetActor = InteractionHitResults.GetData()->GetActor();
	}

	if (NewTargetActor == TargetActor) return;
	TargetActor = NewTargetActor;

	TargetIsChanged.Broadcast(TargetActor);
}

void UInteractionComponent::Interact()
{
	if (!TargetActor) return;

	IInteractionInterface::Execute_Interaction(TargetActor, GetOwner());
}