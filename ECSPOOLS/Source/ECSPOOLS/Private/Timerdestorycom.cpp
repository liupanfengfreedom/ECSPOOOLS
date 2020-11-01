// Fill out your copyright notice in the Description page of Project Settings.


#include "Timerdestorycom.h"
#include "Engine.h"
#include "ECSPBlueprintFunctionLibrary.h"

// Sets default values for this component's properties
UTimerdestorycom::UTimerdestorycom()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTimerdestorycom::ECSBeginplay()
{
	owner = GetOwner();
	//SetComponentTickEnabled(true);
}
void UTimerdestorycom::ECSEndplay()
{
	//SetComponentTickEnabled(false);
}
// Called when the game starts
void UTimerdestorycom::BeginPlay()
{
	Super::BeginPlay();
	// ...
	
}
UTimerdestorycom* UTimerdestorycom::settime(float time)
{
	GetWorld()->GetTimerManager().SetTimer(gamecontroltimer, this, &UTimerdestorycom::timerworker, 10, false, time);
	return this;
}
void UTimerdestorycom::timerworker()
{
	////GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("timerworker!"));
	//GetOwner()->Destroy();
	//UECSPBlueprintFunctionLibrary::ActorRecycle(GetOwner());
	ondestroyactorevent.ExecuteIfBound(owner);
}
// Called every frame
void UTimerdestorycom::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

