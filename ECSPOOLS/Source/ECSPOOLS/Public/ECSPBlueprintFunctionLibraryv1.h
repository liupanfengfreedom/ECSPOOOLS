// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#if !(PLATFORM_IOS)
#include "Engine.h"
#endif
#include "ECSPBlueprintFunctionLibraryv1.generated.h"

/**
 * 
 */
UCLASS()
class ECSPOOLS_API UECSPBlueprintFunctionLibraryv1 : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static TSet<UObject*> cleanuobjectpools;
	static TMap<UClass*, UObject*> uobjectspool;
		template< class T = UObject>
	FUNCTION_NON_NULL_RETURN_START
		static T* GetUobjectsingleton()
		FUNCTION_NON_NULL_RETURN_END
	{
		T* newobj = nullptr;
		UObject** obj = uobjectspool.Find(T::StaticClass());

		if (obj == nullptr)
		{
			newobj = NewObject<T>();
			if (!newobj)
			{
				return nullptr;
			}
			newobj->AddToRoot();
			uobjectspool.Add(T::StaticClass()) = newobj;
			cleanuobjectpools.Add(newobj);
#if !(PLATFORM_IOS)
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString("create a new uobject "));
#endif
		}
		else
		{
#if !(PLATFORM_IOS)
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString("reuse uobject "));
#endif
			newobj = (T*)*obj;
		}
		return  newobj;
	}
};
