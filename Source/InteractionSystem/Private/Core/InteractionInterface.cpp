#include "Core/InteractionInterface.h"

void IInteractionInterface::InteractionFinished(AActor* InteractedObject, AActor* Instigator)
{
	if (!OnInteractionFinished.IsBound()) return;
	OnInteractionFinished.Broadcast(InteractedObject, Instigator);
}