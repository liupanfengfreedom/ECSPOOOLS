// Fill out your copyright notice in the Description page of Project Settings.


#include "ECSPBlueprintFunctionLibraryv1.h"

TMap<UClass*, UObject*> UECSPBlueprintFunctionLibraryv1::uobjectspool;
TSet<UObject*> UECSPBlueprintFunctionLibraryv1::cleanuobjectpools;
