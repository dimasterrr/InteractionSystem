#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

UENUM()
enum class EInteractionType : int8
{
	Trace,
	Sphere
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INTERACTIONSYSTEM_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTargetIsChanged, AActor*, Target);

private:
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	AActor* TargetActor = nullptr;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	bool DrawDebug = false;

	UPROPERTY(EditAnywhere, Category = "Settings")
	EInteractionType InteractionType;

	UPROPERTY(EditAnywhere, Category="Settings")
	float Distance = 200.0f;

	UPROPERTY(BlueprintAssignable)
	FTargetIsChanged TargetIsChanged;

protected:
	virtual void BeginPlay() override;

	
	void PrepareTargetActor();
	void SpawnTrace();
	void SpawnSphere();

public:
	UInteractionComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Interact();
};
