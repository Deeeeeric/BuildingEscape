// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

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

	InitialYaw = GetOwner()->GetActorRotation().Yaw; // get the initial rotation
	CurrentYaw = InitialYaw;
	OpenAngleYaw += InitialYaw; //OpenAngleYaw = OpenAngleYaw + InitialYaw;

	if (PressurePlate)
	{
		// There is a pressure plate
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the open door component on it, but no PressurePlate set."), *GetOwner()->GetName());
	}
	
	// we do not have to eject and set the defaultpawn to actorthatopens, it will do this automaically
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// If the actor overlaps the pressureplate
	if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		// open the door
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds(); // get the time when the door is opened
	}
	else
	{
		// if the door has been open longer than DoorCloseDelay
		if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetActorRotation().ToString());
	// UE_LOG(LogTemp, Warning, TEXT("Yaw is: %f"), GetOwner()->GetActorRotation().Yaw);

	// OpenAngleYaw is 90 degrees so we want to open to to 90.f
	CurrentYaw = FMath::Lerp(CurrentYaw, OpenAngleYaw, DeltaTime * DoorOpenSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	// we want the finish point of the door to be the initialyaw since we are closing it
	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime * DoorCloseSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
}

//For a linear interpolation. Does not slow down the door towards the end
//FInterpConstantTo needs 4 arguments. the interpSpeed 45 states to move 90 degrees in two seconds
//Making it 45 degrees a second
//OpenDoor.Yaw = FMath::FInterpConstantTo(CurrentYaw, OpenAngleYaw, DeltaTime, 45);

//For a more smoother transition than linear interpolation
//OpenDoor.Yaw = FMath::FInterpTo(CurrentYaw, OpenAngleYaw, DeltaTime, 2);

// float MyFloat = 90.f; the f tells the engine that it is a float
// FRotator CurrentRotation = GetOwner()->GetActorRotation();
// CurrentRotation.Yaw = 90.f; // opening the door to 90 degrees
// FRotator OpenDoor(0.f, 90.f, 0.f) is another way to set the door to open 90*