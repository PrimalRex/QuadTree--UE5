// ©2023 Developed by Harris Barra.

#pragma once

#include "CoreMinimal.h"
#include "QTreeNode.h"
#include "QTreeInterface.h"
#include "GameFramework/Actor.h"
#include "QTreeManager.generated.h"

UCLASS(Blueprintable)

class QTREEINTERACTION_API AQTreeManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQTreeManager();
	TSharedPtr<QTreeNode> TreeRoot;
	UPROPERTY()
	TArray<AActor*> SpawnedObjects;
	TArray<TWeakObjectPtr<AActor>> OldQueries;

	FTimerHandle UpdateTimer;

	//Show world bound traces
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuadTree")
	bool bShowDebugQuadTraces = true;
	//Removed unused quads from debug traces
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuadTree")
	bool bFilterEmptyQuadrantsFromDebug = true;
	//Size (X) of manager
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuadTree")
	int32 BoundsX = 1000;
	//Size (Y) of manager
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuadTree")
	int32 BoundsY = 1000;
	//Suggested number of items each quad should hold before partitioning
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuadTree")
	int32 MaximumItemsPerZone = 3;
	//How many subdivisions the quad tree should have
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuadTree")
	int32 MaximumTreeDepth = 5;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SpawnRandomActorsWithinBounds(UClass* ActorClass, int32 AmountToSpawn, const FVector& MinBounds, const FVector& MaxBounds);
	
	UFUNCTION(BlueprintCallable)
	void RefreshQuadTree();

	UFUNCTION()
	void QueryActorsInRadius(const FVector& QueryPoint, float Radius);
};
