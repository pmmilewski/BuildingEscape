// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab action!"))
	auto HitResult = GetFirstPhysicsBodyInReach();

	auto ComponentToGrab = HitResult.GetComponent();
	
	auto ActorHit = HitResult.GetActor();
	if (ActorHit)
	{
		PhysicsHandle->GrabComponentAtLocationWithRotation(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), ComponentToGrab->GetOwner()->GetActorRotation());
	}
}
	

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab released!"))
	PhysicsHandle->ReleaseComponent();
}

void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if(PhysicsHandle)
	{
		
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PhysicsHandle not found in %s!"), *GetOwner()->GetName());
	}
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
		UE_LOG(LogTemp, Error, TEXT("InputComponent not found in %s!"), *GetOwner()->GetName());
	}
}

void UGrabber::UpdateLineTraceEnd()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() 
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	FHitResult Hit{};

	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(Hit, PlayerViewPointLocation, LineTraceEnd, 
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParams);
	if (Hit.GetActor() != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit detected! Object: %s"), *Hit.GetActor()->GetName());
	}
	return Hit;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle->GrabbedComponent)
	{
		UpdateLineTraceEnd();
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}

	
}

