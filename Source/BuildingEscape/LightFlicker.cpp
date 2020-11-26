// Copyright Andrew Tolle 2020


#include "LightFlicker.h"
#include <Runtime\Engine\Classes\Engine\Light.h>
#include <time.h>

// Sets default values for this component's properties
ULightFlicker::ULightFlicker()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULightFlicker::BeginPlay()
{
	Super::BeginPlay();

	srand(time(0));
	
}


// Called every frame
void ULightFlicker::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	((ALight*)GetOwner())->SetBrightness(rand() % 6 + 1);

}

