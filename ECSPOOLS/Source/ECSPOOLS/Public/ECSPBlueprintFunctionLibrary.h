// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ECSPBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ECSPOOLS_API UECSPBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	static int liveactorfrompools;
	static TMap<UClass*, TArray<AActor*>> pools;
public:
	static int getnumberofliveactorfrompool() { return liveactorfrompools; }
public:
	template< class T = UActorComponent>
	FUNCTION_NON_NULL_RETURN_START
		static T*  AddComponent(AActor* Outer)
		FUNCTION_NON_NULL_RETURN_END
	{
		T * ac = Cast<T>(Outer->GetComponentByClass(T::StaticClass()));
		if (ac)
		{
			return ac;
		}
		T* NewComp = NewObject<T>(Outer, T::StaticClass());
		if (!NewComp)
		{
			return nullptr;
		}
		NewComp->RegisterComponent();        //You must ConstructObject with a valid Outer that has world, see above	 
		//FAttachmentTransformRules atf(EAttachmentRule::KeepRelative,false);
		//NewComp->AttachToComponent(Outer->GetRootComponent(), atf);
		Outer->AddOwnedComponent(NewComp);
		return  NewComp;
	}
	template< class T = UActorComponent>
	FUNCTION_NON_NULL_RETURN_START
		static bool RemoveComponent(AActor* Outer)
		FUNCTION_NON_NULL_RETURN_END
	{
		T  * ac = Cast<T>(Outer->GetComponentByClass(T::StaticClass()));
		if (ac)
		{
			ac->DestroyComponent();
		}
		return true;
	}
	template< class T = AActor>
	   FUNCTION_NON_NULL_RETURN_START
		UFUNCTION(BlueprintCallable)
		static T* GetActorFromPool(UClass* tclass, UWorld* world, FTransform transform)
		FUNCTION_NON_NULL_RETURN_END
	{
		liveactorfrompools++;
		AActor * value = nullptr;
		TArray<AActor*>* actorarray = pools.Find(tclass);
		if (actorarray != nullptr)
		{
			int num = actorarray->Num();
			if (num > 0)
			{
				value = (*actorarray)[num - 1];
				actorarray->RemoveAt(num - 1);
				//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("actor from pool"));
			}
		}
		if (value == nullptr)
		{
			struct FActorSpawnParameters SpawnInfo;
			SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			value = world->SpawnActor<AActor>(tclass, transform, SpawnInfo);
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("actor from spawn"));
		}
		value->SetActorTransform(transform);
		// Hides visible components
		value->SetActorHiddenInGame(false);
		// Disables collision components
		value->SetActorEnableCollision(true);
		// Stops the Actor from ticking
		value->SetActorTickEnabled(true);
		return Cast<T>(value);
	}
	   template< class T = AActor>
	   FUNCTION_NON_NULL_RETURN_START
		   UFUNCTION(BlueprintCallable)
		   static T* GetActorFromPool(UWorld* world ,FTransform transform)
		   FUNCTION_NON_NULL_RETURN_END
	   {
		   liveactorfrompools++;
		   AActor * value = nullptr;
		   TArray<AActor*>* actorarray = pools.Find(T::StaticClass());
		   if (actorarray != nullptr)
		   {
			   int num = actorarray->Num();
			   if (num > 0)
			   {
				   value = (*actorarray)[num - 1];
				   actorarray->RemoveAt(num - 1);
				   //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("actor from pool"));
			   }
		   }
		   if (value == nullptr)
		   {
			   struct FActorSpawnParameters SpawnInfo;
			   SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;			   
			   value = world->SpawnActor<AActor>(T::StaticClass(), transform, SpawnInfo);
			   //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("actor from spawn"));
		   }
		   value->SetActorTransform(transform);
		   // Hides visible components
		   value->SetActorHiddenInGame(false);
		   // Disables collision components
		   value->SetActorEnableCollision(true);
		   // Stops the Actor from ticking
		   value->SetActorTickEnabled(true);
		   return Cast<T>(value);
	   }
	UFUNCTION(BlueprintCallable)
		static bool ActorRecycle(AActor *pa)
	{
		liveactorfrompools--;
		// Hides visible components
		pa->SetActorHiddenInGame(true);
		// Disables collision components
		pa->SetActorEnableCollision(false);
		// Stops the Actor from ticking
		pa->SetActorTickEnabled(false);
		UClass*  uclass = pa->GetClass();
		TArray<AActor*>* actorarray = pools.Find(uclass);
		if (actorarray == nullptr)
		{
			pools.Add(uclass, TArray<AActor*>());
		}
		actorarray = pools.Find(uclass);
		actorarray->AddUnique(pa);
		return true;
	}

};
