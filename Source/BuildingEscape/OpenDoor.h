// Copyright Andrew Tolle 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

private:
	enum DoorState { Open, Closed, Opening, Closing };

	void SetupAudioComponent();
	void PlaySoundIfDoorOpenedOrClosed();

	AActor* Owner = nullptr;
	float InitialYaw;
	float TotalMassOfActors() const;
	DoorState DoorState = Closed;

	UPROPERTY(EditAnywhere)
	float MassToOpenDoor = 20.0f;

	UPROPERTY(EditAnywhere)
	float TargetYaw = 90.0f;

	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = 2.0f;

	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed = 2.0f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr;


protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OpenDoor(float DeltaTime);

	void CloseDoor(float DeltaTime);
};
