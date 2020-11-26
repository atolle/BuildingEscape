// Copyright Andrew Tolle 2020

#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"
#include "Grabber.h"

UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetupInputComponent();
}

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (DebugMode)
		DrawDebugTraceLine();

	if (PhysicsHandle && PhysicsHandle->GrabbedComponent)
		PhysicsHandle->SetTargetLocation(GetPlayerReach());
}

void UGrabber::DrawDebugTraceLine()
{
	DrawDebugLine(
		GetWorld(),
		GetPlayerViewPointLocation(),
		GetPlayerReach(),
		FColor(0, 255, 0),
		false,
		0.0f,
		0,
		5.0f
	);
}

void UGrabber::Grab()
{
	if (!PhysicsHandle)
		return;

	UE_LOG(LogTemp, Warning, TEXT("Grabber key pressed"));

	FHitResult HitResult = GetFirstPhysicsBodyInReach();

	if (HitResult.GetActor())
	{
		UE_LOG(LogTemp, Display, TEXT("HitResult: %s"), *HitResult.GetActor()->GetName());

		UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();		
		PhysicsHandle->GrabComponentAtLocation(ComponentToGrab, NAME_None, GetPlayerReach());
	}		
}

void UGrabber::Release()
{
	if (!PhysicsHandle)
		return;

	UE_LOG(LogTemp, Warning, TEXT("Grabber key released"));

	if (PhysicsHandle->GrabbedComponent)
		PhysicsHandle->ReleaseComponent();
}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (!PhysicsHandle)
		UE_LOG(LogTemp, Error, TEXT("No PhysicsHandle exists on %s"), *(GetOwner()->GetName()));
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No InputComponent exists on %s"), *(GetOwner()->GetName()));
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayerViewPointLocation(),
		GetPlayerReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	return Hit;
}

FVector UGrabber::GetPlayerReach() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}

FVector UGrabber::GetPlayerViewPointLocation() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	return PlayerViewPointLocation;
}