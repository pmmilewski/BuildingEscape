// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"



// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UOpenDoor::OpenDoor()
{
	Owner->SetActorRotation(Owner->GetActorRotation() + FRotator(0.f, OpenedDoorAngle, 0.f));
}

void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(Owner->GetActorRotation() - FRotator(0.f, OpenedDoorAngle, 0.f));
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpens) && !IsOpened && LeftPressurePlate)
	{
		OpenDoor();
		IsOpened = true;
		LeftPressurePlate = false;
		LastTimeOpened = GetWorld()->GetTimeSeconds();
	}

	if (PressurePlate && !PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		LeftPressurePlate = true;
	}

	if (GetWorld()->GetTimeSeconds() - LastTimeOpened > CloseDelayTime && IsOpened)
	{
		CloseDoor();
		IsOpened = false;
	}
}

