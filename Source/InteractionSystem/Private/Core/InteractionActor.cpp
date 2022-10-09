#include "Core/InteractionActor.h"

void AInteractionActor::SetInteractionFinished(AActor* TargetInstigator)
{
	InteractionFinished(this, TargetInstigator);
}
