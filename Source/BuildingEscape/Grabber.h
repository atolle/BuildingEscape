// Copyright Andrew Tolle 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGrabber();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	void Grab();
	void Release();
	void FindPhysicsHandle();
	void SetupInputComponent();
	void DrawDebugTraceLine();
	FHitResult GetFirstPhysicsBodyInReach() const;
	FVector GetPlayerReach() const;
	FVector GetPlayerViewPointLocation() const;

	UPROPERTY(EditAnywhere)
	bool DebugMode = false;
	
	float Reach = 200.0f;

	UPROPERTY()
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UPROPERTY()
	UInputComponent* InputComponent = nullptr;

};
