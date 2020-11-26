// Copyright Andrew Tolle 2020


#include "OpenDoor.h"
#include "GameFramework/Actor.h"

UOpenDoor::UOpenDoor()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();

	InitialYaw = Owner->GetActorRotation().Yaw;

	TargetYaw += Owner->GetActorRotation().Yaw;

	if (!PressurePlate)
		UE_LOG(LogTemp, Error, TEXT("%s has OpenDoor component but no PressurePlate set."), *GetOwner()->GetName());

	SetupAudioComponent();
}

void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);	

	if (DoorState != Open && TotalMassOfActors() >= MassToOpenDoor)
		OpenDoor(DeltaTime);

	if (DoorState != Closed && TotalMassOfActors() < MassToOpenDoor)
		CloseDoor(DeltaTime);

	//PlaySoundIfDoorOpenedOrClosed();
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	if (!Owner)
		return;

	if (DoorState == Closed)
	{
		if (AudioComponent)
			AudioComponent->Play();

		DoorState = Opening;
	}

	float PreviousYaw = Owner->GetActorRotation().Yaw;

	Owner->SetActorRotation(FRotator(0.0f, FMath::FInterpTo(Owner->GetActorRotation().Yaw, TargetYaw, DeltaTime, DoorOpenSpeed), 0.0f));	

	float CurrentYaw = Owner->GetActorRotation().Yaw;

	UE_LOG(LogTemp, Warning, TEXT("PreviousYaw: %f CurrentYaw: %f"), PreviousYaw, CurrentYaw);

	if (PreviousYaw == CurrentYaw)
		DoorState = Open;
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	if (!Owner)
		return;

	if (DoorState == Open)
		DoorState = Closing;

	float PreviousYaw = Owner->GetActorRotation().Yaw;

	Owner->SetActorRotation(FRotator(0.0f, FMath::FInterpTo(Owner->GetActorRotation().Yaw, InitialYaw, DeltaTime, DoorCloseSpeed), 0.0f));

	float CurrentYaw = Owner->GetActorRotation().Yaw;

	UE_LOG(LogTemp, Warning, TEXT("PreviousYaw: %f CurrentYaw: %f"), PreviousYaw, CurrentYaw);

	if (PreviousYaw == CurrentYaw)
	{
		if (AudioComponent)
			AudioComponent->Play();

		DoorState = Closed;
	}
}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.0f;

	if (!PressurePlate)
		return TotalMass;

	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for (AActor* OverlappingActor : OverlappingActors)		
		TotalMass += OverlappingActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();

	return TotalMass;
}

void UOpenDoor::SetupAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();	

	if (!AudioComponent)
		UE_LOG(LogTemp, Error, TEXT("No AudioComponent exists on %s"), *(GetOwner()->GetName()));
}

//void UOpenDoor::PlaySoundIfDoorOpenedOrClosed()
//{
//	if (!AudioComponent)
//		return;
//	
//	float CurrentYaw = Owner->GetActorRotation().Yaw;
//
//	if ((CurrentYaw == 0.0f && PreviousYaw != 0.0f) || (CurrentYaw != 0.0f && PreviousYaw == 0.0f))
//		AudioComponent->Play();
//
//	PreviousYaw = CurrentYaw;
//}