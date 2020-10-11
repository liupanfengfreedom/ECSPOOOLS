// Fill out your copyright notice in the Description page of Project Settings.


#include "ECSPBlueprintFunctionLibrary.h"
TMap<UClass*, TArray<AActor*>> UECSPBlueprintFunctionLibrary::pools;
int UECSPBlueprintFunctionLibrary::liveactorfrompools=0;
