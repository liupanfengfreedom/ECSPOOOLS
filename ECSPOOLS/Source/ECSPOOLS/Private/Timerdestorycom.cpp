// Fill out your copyright notice in the Description page of Project Settings.


#include "Timerdestorycom.h"
#include "Engine.h"
#include "ECSPBlueprintFunctionLibrary.h"

// Sets default values for this component's properties
UTimerdestorycom::UTimerdestorycom()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTimerdestorycom::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}
UTimerdestorycom* UTimerdestorycom::PublicBeginPlay(float lifespan)
{
	GetWorld()->GetTimerManager().SetTimer(gamecontroltimer, this, &UTimerdestorycom::timerworker, 10, false, lifespan);
	return this;
}
void UTimerdestorycom::timerworker()
{
	////GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("timerworker!"));
	//GetOwner()->Destroy();
	//UECSPBlueprintFunctionLibrary::ActorRecycle(GetOwner());
	ondestroyactorevent.ExecuteIfBound(GetOwner());
}
// Called every frame
void UTimerdestorycom::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

