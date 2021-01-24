// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "ECSInterface.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ECSPBlueprintFunctionLibrary.generated.h"
class UUserWidget;
class ECSinterface
{
public:
	virtual void ECSBeginplay() = 0;
	virtual void ECSEndplay()=0;
};
/**
 * 
 */
UCLASS()
class ECSPOOLS_API UECSPBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	static int liveactorfrompools;
	static TMap<FString, UObject*> assetpools;

	static TMap<FString, UClass*> UserWidgetClasspools;
	static TMap<UClass*, TArray<UUserWidget*>> UserWidgetpools;

	static TMap<UClass*, TArray<AActor*>> pools;
	static TMap<UClass*, TArray<UObject*>> componentpools;
	static TSet<UObject*> cleanuobjectpools;


public:
	static int getnumberofliveactorfrompool() { return liveactorfrompools; }
	UFUNCTION(BlueprintCallable, Category = ECSPBlueprint)
		static UUserWidget* GetUserWidgetFromPool(FString assetpath, UWorld* outer);
	UFUNCTION(BlueprintCallable, Category = ECSPBlueprint)
		static void UserWidgetRecycle(UUserWidget* obj);
	UFUNCTION(BlueprintCallable, Category = ECSPBlueprint)
		static void ResetECSPool();
public:
	template< class T = UActorComponent>
	FUNCTION_NON_NULL_RETURN_START
		UFUNCTION(BlueprintCallable, Category = ECSPBlueprint)
		static T*  AddComponent(AActor* Outer)
		FUNCTION_NON_NULL_RETURN_END
	{
		T * ac = Cast<T>(Outer->GetComponentByClass(T::StaticClass()));
		if (ac)
		{
			return ac;
		}
		T*  NewComp = nullptr;
		TArray<UObject*>* actorarray = componentpools.Find(T::StaticClass());
		if (actorarray != nullptr)
		{
			int num = actorarray->Num();
			if (num > 0)
			{
				NewComp = (T * )(*actorarray)[num - 1];
				actorarray->RemoveAt(num - 1);
				NewComp->Rename(TEXT(""), Outer);
				//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Component from pool"));
			}
		}
		if (NewComp == nullptr)
		{
			 NewComp = NewObject<T>(Outer, T::StaticClass());
			if (!NewComp)
			{
				return nullptr;
			}
			NewComp->AddToRoot();
			cleanuobjectpools.Add(NewComp);
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString("Component from NewObject: ").Append(T::StaticClass()->GetDefaultObjectName().ToString()));
		}

		NewComp->RegisterComponent();        //You must ConstructObject with a valid Outer that has world, see above	 
		//FAttachmentTransformRules atf(EAttachmentRule::KeepRelative,false);
		//NewComp->AttachToComponent(Outer->GetRootComponent(), atf);

		Outer->AddOwnedComponent(NewComp);
		ECSinterface* ep = (ECSinterface*)NewComp;
		ep->ECSBeginplay();
		NewComp->SetComponentTickEnabled(true);
		return  NewComp;
	}
	template< class T = UActorComponent>
	FUNCTION_NON_NULL_RETURN_START
		UFUNCTION(BlueprintCallable, Category = ECSPBlueprint)
		static bool RemoveComponent(AActor* Outer)
		FUNCTION_NON_NULL_RETURN_END
	{
		T  * ac = Cast<T>(Outer->GetComponentByClass(T::StaticClass()));
		if (ac)
		{
			ECSinterface* ep = (ECSinterface*)ac;
			ep->ECSEndplay();
			ac->SetComponentTickEnabled(false);
			Outer->RemoveInstanceComponent(ac);
			Outer->RemoveOwnedComponent(ac);
			ac->UnregisterComponent();
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Component from pool"));
			//ac->DestroyComponent();
			componentpools.FindOrAdd(T::StaticClass()).AddUnique(ac);
			return true;
		}
		return false;
	}
	template< class T = AActor>
	   FUNCTION_NON_NULL_RETURN_START
		   UFUNCTION(BlueprintCallable, Category = ECSPBlueprint)
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
		
		IECSInterface* ep = Cast<IECSInterface>(value);
		if (ep)
		{
			ep->Execute_ECSBeginplay(value);
		}
		return Cast<T>(value);
	}
	   template< class T = AActor>
	   FUNCTION_NON_NULL_RETURN_START
		   UFUNCTION(BlueprintCallable, Category = ECSPBlueprint)
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
		   IECSInterface* ep = Cast<IECSInterface>(value);
		   if (ep)
		   {
			   ep->Execute_ECSBeginplay(value);
		   }
		   return Cast<T>(value);
	   }
	   UFUNCTION(BlueprintCallable, Category = ECSPBlueprint)
		static bool ActorRecycle(AActor *pa)
	{
		liveactorfrompools--;
		IECSInterface* ep = Cast<IECSInterface>(pa);
		if (ep)
		{
			ep->Execute_ECSEndplay(pa);
		}
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


	   UFUNCTION(BlueprintCallable, Category = ECSPBlueprint)
		static UObject* GetAssetFromPool(FString assetpath)
	{
		UObject** p = assetpools.Find(assetpath);
		if (p)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("GetAssetFromPool"));
			return *p;
		}
		else
		{
			UObject* ske = LoadObject<UObject>(nullptr, *assetpath);
			//ske->SetFlags(EObjectFlags::RF_Standalone);
			ske->AddToRoot();
			cleanuobjectpools.Add(ske);
			assetpools.Add(assetpath) = ske;
			return ske;
		}
	}



};
