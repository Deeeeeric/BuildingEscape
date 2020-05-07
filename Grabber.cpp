// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();

	SetupInputComponent();
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::GrabComponent);
		// &UGrabber::Grab the address of the Grab function thats part of the UGrabber class
		// &UGrabber::Grab does not have Grab() because we are not calling the function,
		// we are telling it where to go in order to call it when we press the button
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::ReleaseComponent);
	}
}

// Check for PhysicsHandle components
void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle)
	{
		// PhysicsHandle is found
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No physics handle component found on %s!"), *GetOwner()->GetName());
	}
}

void UGrabber::GrabComponent()
{
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent *ComponentToGrab = HitResult.GetComponent();

	//If we hit something then attach the physics handle.
	if (HitResult.GetActor())
	{
		//TODO attach physics handle.
		PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None, // we are not dealing with a skeletal mesh, so the name is NONE
			GetTheGrabReach());
	}
}

FVector UGrabber::GetTheGrabReach() const
{
	// Get players viewpoint (their exact location and rotation in the world
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	// how to Get the players viewfront from toptobottom
	// GetPlayerViewPoint needs 2 OUT arguments, one FVector one FRotator, which we have initialised above
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);

	// Draw a line from player showing a reach
	// This line will point straight up
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	return LineTraceEnd;
}

void UGrabber::ReleaseComponent()
{
	PhysicsHandle->ReleaseComponent();
}

FVector UGrabber::PlayerWorldPosition() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);

	return PlayerViewPointLocation;
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// If the PhysicsHandle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		// Move the obect we are holding
		PhysicsHandle->SetTargetLocation(GetTheGrabReach());
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FHitResult Hit;
	// Ray-cast out a certain distance (sending out an arm/reach)
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerWorldPosition(),
		GetTheGrabReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams);

	return Hit;
}

