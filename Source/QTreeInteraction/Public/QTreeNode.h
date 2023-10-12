// ©2023 Developed by Harris Barra.

#pragma once

#include "CoreMinimal.h"
#include "QTreeInterface.h"
#include "GameFramework/Actor.h"

/**
 * 
 */
class QTREEINTERACTION_API QTreeNode : public IQTreeInterface
{
public:
	int32 TreeDepth;
	int32 MaxPerZone;
	bool bFilterEmptyQuads;
	struct FBounds
	{
		FVector Min;
		FVector Max;
	};
	enum class ENodeQuadrant : uint8 { NW, NE, SW, SE,};
	struct FNodeData
	{
		int32 Level;
		bool bIsLeaf;
		FBounds Bounds;
		bool bIsWithinPlayerRange;
		TArray<TWeakObjectPtr<AActor>> Objects;
	};
	
	ENodeQuadrant Quadrant;
	FNodeData NodeData;
	TSharedPtr<QTreeNode> RootNode;
	TArray<TSharedPtr<QTreeNode>> ChildNodes;
	UWorld* WorldRef = nullptr;
	
	QTreeNode(const TSharedPtr<QTreeNode>& MainNode = nullptr, uint32 Level = 0, const FBounds& NewBounds = {FVector(0, 0, 0), FVector(0, 0, 0)},
		ENodeQuadrant NewQuadrant = ENodeQuadrant::NW, const int32& MaximumItemsPerZone = 3, const int32& MaximumTreeDepth = 5, bool bFilterUnusedQuads = true)
	{
		RootNode = MainNode;
		Quadrant = NewQuadrant;
		NodeData.Level = Level;
		NodeData.Bounds = NewBounds;
		NodeData.bIsLeaf = true;
		NodeData.bIsWithinPlayerRange = false;
		MaxPerZone = MaximumItemsPerZone;
		TreeDepth = MaximumTreeDepth;
		bFilterEmptyQuads = bFilterUnusedQuads;
		//Use 4 child nodes as we want to divide into 4 quadrants
		ChildNodes.Init(MainNode, 4);
	};
	
	~QTreeNode()
	{
		//Destroy everything
		RootNode = nullptr;
		NodeData.Objects.Empty();
		ChildNodes.Empty();
	};

	UFUNCTION()
	bool AABBInclusion(const ENodeQuadrant QuadrantToCheck, TWeakObjectPtr<AActor> Object, const FVector& QueryPoint) const;
	
	UFUNCTION()
	void Insertion(TWeakObjectPtr<AActor> Object);

	UFUNCTION()
	void Partition();

	UFUNCTION()
	TArray<TWeakObjectPtr<AActor>> RangeQuery(const FVector& QueryPoint, float Radius, TArray<QTreeNode*>& ObjectNode);
	
	UFUNCTION()
	void DrawBounds(UWorld* World, float Time, float Thickness);
	
};
