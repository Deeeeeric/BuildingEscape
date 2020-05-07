// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	float Reach = 100.f;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;	
	
	void GrabComponent();
	void ReleaseComponent();
	void FindPhysicsHandle();
	void SetupInputComponent();
	void ShowActorLocationAndRotation();
	void DrawDebugLine();
	void GetTheHitObject();
	
	// Return the first actor within reach with a physics body
	FHitResult GetFirstPhysicsBodyInReach() const;

	// Return the Line Trace End
	FVector GetTheGrabReach() const;

	// Get Players world position
	FVector PlayerWorldPosition() const;
};
