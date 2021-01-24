// Fill out your copyright notice in the Description page of Project Settings.


#include "ECSPBlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Widget.h"
#include "Blueprint/UserWidget.h"
#include "ECSPBlueprintFunctionLibraryv1.h"

TMap<UClass*, TArray<AActor*>> UECSPBlueprintFunctionLibrary::pools;
TMap<UClass*, TArray<UObject*>> UECSPBlueprintFunctionLibrary::componentpools;
int UECSPBlueprintFunctionLibrary::liveactorfrompools=0;
TMap<FString, UObject*> UECSPBlueprintFunctionLibrary::assetpools;

TMap<FString, UClass*> UECSPBlueprintFunctionLibrary::UserWidgetClasspools;
TMap<UClass*, TArray<UUserWidget*>> UECSPBlueprintFunctionLibrary::UserWidgetpools;
TSet<UObject*> UECSPBlueprintFunctionLibrary::cleanuobjectpools;

UUserWidget* UECSPBlueprintFunctionLibrary::GetUserWidgetFromPool(FString assetpath, UWorld* outer)
{
	UUserWidget* value = nullptr;
	UClass** widgetclass = UserWidgetClasspools.Find(assetpath);
	if (widgetclass != nullptr)
	{
		TArray<UUserWidget*>* UserWidgetArray = UserWidgetpools.Find(*widgetclass);
		if (UserWidgetArray != nullptr)
		{
			int num = UserWidgetArray->Num();
			if (num > 0)
			{
				value = (*UserWidgetArray)[num - 1];
				UserWidgetArray->RemoveAt(num - 1);
				//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Widget from pool"));
			}
			else
			{
				value = CreateWidget<UUserWidget>(outer, *widgetclass, FName(""));
				value->AddToRoot();
				cleanuobjectpools.Add(value);
				//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Widget from CreateWidget"));
			}
		}
		else
		{
			value = CreateWidget<UUserWidget>(outer, *widgetclass, FName(""));
			value->AddToRoot();
			cleanuobjectpools.Add(value);
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Widget from CreateWidget"));
		}
	}
	else
	{
		UClass* uclass = LoadClass<UUserWidget>(NULL, *assetpath);
		uclass->AddToRoot();
		cleanuobjectpools.Add(uclass);
		UserWidgetClasspools.Add(assetpath, uclass);
		value = CreateWidget<UUserWidget>(outer, uclass);
		value->AddToRoot();
		cleanuobjectpools.Add(value);
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Widget from CreateWidget"));
	}
	return value;
}
void UECSPBlueprintFunctionLibrary::UserWidgetRecycle(UUserWidget* obj)
{
	obj->RemoveFromParent();
	UClass* uclass = obj->GetClass();

	TArray<UUserWidget*>* UserWidgetArray = UserWidgetpools.Find(uclass);
	if (UserWidgetArray == nullptr)
	{
		UserWidgetpools.Add(uclass, TArray<UUserWidget*>());
	}
	UserWidgetArray = UserWidgetpools.Find(uclass);
	UserWidgetArray->AddUnique(obj);
}

void UECSPBlueprintFunctionLibrary::ResetECSPool()
{
	for (auto var : cleanuobjectpools)
	{
		var->RemoveFromRoot();
	}
	for (auto var : UECSPBlueprintFunctionLibraryv1::cleanuobjectpools)
	{
		var->RemoveFromRoot();
	}
	pools.Empty();
	UECSPBlueprintFunctionLibraryv1::uobjectspool.Empty();
	componentpools.Empty();
	assetpools.Empty();
	UserWidgetClasspools.Empty();
	UserWidgetpools.Empty();
	cleanuobjectpools.Empty();
	UECSPBlueprintFunctionLibraryv1::cleanuobjectpools.Empty();
}
