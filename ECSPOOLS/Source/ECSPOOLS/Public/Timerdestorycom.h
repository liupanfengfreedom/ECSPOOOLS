// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Timerdestorycom.generated.h"

DECLARE_DELEGATE_OneParam(FOnDestroyActor, AActor*);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ECSPOOLS_API UTimerdestorycom : public USceneComponent
{
	GENERATED_BODY()
	FTimerHandle gamecontroltimer;
	void timerworker();
public:	
	// Sets default values for this component's properties
	UTimerdestorycom();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UTimerdestorycom* PublicBeginPlay(float lifespan);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:
	FOnDestroyActor ondestroyactorevent;
		
};
