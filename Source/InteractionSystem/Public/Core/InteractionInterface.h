#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

UINTERFACE()
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class INTERACTIONSYSTEM_API IInteractionInterface
{
	GENERATED_BODY()
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnInteractionFinished, AActor*, AActor*)

public:
	FOnInteractionFinished OnInteractionFinished;

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Interaction(AActor* Instigator);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	bool CanInteract() const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	FText GetInteractTitle() const;

	virtual void InteractionFinished(AActor* InteractedObject, AActor* Instigator);
};
