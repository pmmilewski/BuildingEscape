// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#define OUT


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	if(PressurePlate == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pressure Plate not set!"));
	}
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate == nullptr){ return;}
	if (GetTotalMassOnPressurePlate() > TriggerMass)
	{
		OnOpen.Broadcast();
	}
	else
	{
		OnClose.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOnPressurePlate()
{
	float TotalMass = 0.f;
	if (PressurePlate == nullptr){ return TotalMass;}
	TArray<AActor*> ActorsOnPressurePlate;
	PressurePlate->GetOverlappingActors(OUT ActorsOnPressurePlate);

	for(const auto& Actor : ActorsOnPressurePlate)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}

