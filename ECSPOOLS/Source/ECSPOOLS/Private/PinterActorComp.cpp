// Fill out your copyright notice in the Description page of Project Settings.


#include "PinterActorComp.h"
#include "Engine.h"

// Sets default values for this component's properties
UPinterActorComp::UPinterActorComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPinterActorComp::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPinterActorComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FHitResult hr;
	bool b = GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, true, hr);
	if (b)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("true!"));
		GetOwner()->SetActorLocation(hr.Location);
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("false!"));

	}
	// ...
}

