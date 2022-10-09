#pragma once

#include "CoreMinimal.h"
#include "InteractionInterface.h"
#include "GameFramework/Actor.h"
#include "InteractionActor.generated.h"

UCLASS()
class INTERACTIONSYSTEM_API AInteractionActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	void SetInteractionFinished(AActor* TargetInstigator);
};
