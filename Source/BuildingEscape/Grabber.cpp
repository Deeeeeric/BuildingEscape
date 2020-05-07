// Fill out your copyright notice in the Description page of Project Settings.

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Grabber.h"

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
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		// &UGrabber::Grab the address of the Grab function thats part of the UGrabber class
		// &UGrabber::Grab does not have Grab() because we are not calling the function,
		// we are telling it where to go in order to call it when we press the button
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
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

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber Pressed."));
	
	//TODO only ray-cast when key is pressed
	// Try and reach any actors with a physics body collision channel set.
	//If we hit something then attach the physics handle.
	//TODO attach physics handle.
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber Released"));
	//TODO remove/release physics handle
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// If PhysicsHandle attached,
	// Move object we are holding
	GetFirstPhysicsBodyInReach();

	
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	// Get players viewpoint (their exact location and rotation in the world)
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	// how to Get the players viewfront from toptobottom
	// GetPlayerViewPoint needs 2 OUT arguments, one FVector one FRotator, which we have initialised above
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);

	// we need to pass strings so we use the %s standard operator.
	// *overloaded function because we are turning an FVector/FRotator to an FString
	// UE_LOG(LogTemp, Warning, TEXT("Location:%s, Rotation:%s"),
	// 	   *PlayerViewPointLocation.ToString(), *PlayerViewPointRotation.ToString()
	// );

	// Draw a line from player showing a reach

	// this line will point straight up
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation, // start from the player
		LineTraceEnd,
		FColor(0, 255, 0), // green line
		false,
		0.f,
		0,
		5.f // 5 pixels in width
	);

	FHitResult Hit;
	// Ray-cast out a certain distance (sending out an arm/reach)
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams);

	// Logging out to test
	AActor *ActorHit = Hit.GetActor(); // get the actor from Hit, if we hit anything, it will be stored in ActorHit

	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace has hit %s"), *ActorHit->GetName());
	}
}
