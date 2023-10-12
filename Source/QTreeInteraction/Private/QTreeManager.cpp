// ©2023 Developed by Harris Barra.

#include "QTreeInteraction/Public/QTreeManager.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AQTreeManager::AQTreeManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AQTreeManager::BeginPlay()
{
	Super::BeginPlay();
	const FVector MinBound = FVector(-BoundsX, -BoundsY, -0) + GetActorLocation();
	const FVector MaxBound = FVector(BoundsX, BoundsY, 0) + GetActorLocation();
	TreeRoot = MakeShareable(new QTreeNode(nullptr, 0, QTreeNode::FBounds{MinBound, MaxBound}, QTreeNode::ENodeQuadrant::NW, MaximumItemsPerZone, MaximumTreeDepth, bFilterEmptyQuadrantsFromDebug));
	if(TreeRoot)
	{
		TreeRoot->WorldRef = GetWorld();
		GetWorld()->GetTimerManager().SetTimer(UpdateTimer, this, &AQTreeManager::RefreshQuadTree, .1f, true);
	}
	
}

// Called every frame
void AQTreeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AQTreeManager::SpawnRandomActorsWithinBounds(UClass* ActorClass, int32 AmountToSpawn, const FVector& MinBounds, const FVector& MaxBounds)
{
	if (!ActorClass || AmountToSpawn <= 0)
	{
		return;
	}

	for (int i = 0; i < AmountToSpawn; i++)
	{
		const FVector SpawnLocation = FMath::RandPointInBox(FBox(MinBounds, MaxBounds));
		const FTransform SpawnTransform = FTransform(SpawnLocation);
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorClass, SpawnTransform);
		SpawnedObjects.Add(SpawnedActor);
		if(TreeRoot)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Inserting New Object!"));	
			TreeRoot->Insertion(SpawnedActor);
		}
	}
}

void AQTreeManager::RefreshQuadTree()
{
	if(TreeRoot && bShowDebugQuadTraces)
	{
		const FVector MinBound = FVector(-BoundsX, -BoundsY, -0) + GetActorLocation();
		const FVector MaxBound = FVector(BoundsX, BoundsY, 0) + GetActorLocation();
		TreeRoot->DrawBounds(GetWorld(), .1f, 3.f);
	}
}

void AQTreeManager::QueryActorsInRadius(const FVector& QueryPoint, float Radius)
{
	if (TreeRoot)
	{
		TArray<QTreeNode*> FetchedNodes;
		TArray<TWeakObjectPtr<AActor>> FetchedQueries = TreeRoot->RangeQuery(QueryPoint, Radius, FetchedNodes);
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, *FString::Printf(TEXT("Queried %d Nodes!"), FetchedNodes.Num()));

		for (QTreeNode* Node : OldNodes)
		{
			if (!FetchedNodes.Contains(Node))
			{
				Node->NodeData.bIsWithinPlayerRange = false;
			}
		}

		for (QTreeNode* Node : FetchedNodes)
		{
			if (!OldNodes.Contains(Node))
			{
				Node->NodeData.bIsWithinPlayerRange = true;
			}
		}

		OldNodes = FetchedNodes;

		//Handle queries that no longer exist
		for (TWeakObjectPtr<AActor>& Object : OldQueries)
		{
			if (!FetchedQueries.Contains(Object))
			{
				if (IQTreeInterface* QTreeInterface = Cast<IQTreeInterface>(Object))
				{
					QTreeInterface->OnUnqueriedBySearch_Implementation();
				}
			}
		}

		//Handle new queries
		for (TWeakObjectPtr<AActor>& Object : FetchedQueries)
		{
			if (!OldQueries.Contains(Object))
			{
				if (IQTreeInterface* QTreeInterface = Cast<IQTreeInterface>(Object))
				{
					QTreeInterface->OnQueriedBySearch_Implementation();
				}
			}
		}
		
		OldQueries = FetchedQueries;
		
		for (TWeakObjectPtr<AActor>& Object : FetchedQueries)
		{
			if (OldQueries.Contains(Object))
			{
				if (IQTreeInterface* QTreeInterface = Cast<IQTreeInterface>(Object))
				{
					//Handle queries that havent changed if you so wish
				}
			}
		}
	}
}

