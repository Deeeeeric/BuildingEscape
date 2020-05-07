// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldPosition.h"
#include "GameFrameWork/Actor.h"

// Sets default values for this component's properties
UWorldPosition::UWorldPosition()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWorldPosition::BeginPlay()
{
	Super::BeginPlay();

	// FString Log = TEXT("Hello!");
	// FString* PtrLog = &Log;

	// Log.Len();

	// (*PtrLog).Len();
	// PtrLog->Len();

	// UE_LOG(LogTemp, Warning, TEXT("%s"), **PtrLog);	

	FString ObjectName = GetOwner()->GetName(); // Get the name of the object
	UE_LOG(LogTemp, Warning, TEXT("The object name is: %s"), *ObjectName); // Display object name

	// GetActorLocation is an FVector, so we need ToString to convert it to a string
	FString ObjectPosition = GetOwner()->GetActorLocation().ToString(); // Get object location
	UE_LOG(LogTemp, Warning, TEXT("The location of the object is: %s"), *ObjectPosition); // Display object location
}


// Called every frame
void UWorldPosition::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

