// Fill out your copyright notice in the Description page of Project Settings.


#include "ECSPBlueprintFunctionLibrary.h"
TMap<UClass*, TArray<AActor*>> UECSPBlueprintFunctionLibrary::pools;
TMap<UClass*, TArray<UObject*>> UECSPBlueprintFunctionLibrary::componentpools;
int UECSPBlueprintFunctionLibrary::liveactorfrompools=0;
TMap<FString, UObject*> UECSPBlueprintFunctionLibrary::assetpools;
