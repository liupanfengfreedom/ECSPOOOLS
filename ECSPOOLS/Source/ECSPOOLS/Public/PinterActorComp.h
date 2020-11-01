// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ECSPBlueprintFunctionLibrary.h"
#include "PinterActorComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ECSPOOLS_API UPinterActorComp : public UActorComponent, public ECSinterface
{
	GENERATED_BODY()
		AActor* owner;
public:	
	// Sets default values for this component's properties
	UPinterActorComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	virtual void ECSBeginplay() override;
	virtual void ECSEndplay()  override;
};
