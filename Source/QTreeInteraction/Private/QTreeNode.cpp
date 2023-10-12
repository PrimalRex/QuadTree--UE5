// ©2023 Developed by Harris Barra.

#include "QTreeInteraction/Public/QTreeNode.h"
#include "Runtime/Core/Public/Async/ParallelFor.h"
#include "Kismet/KismetSystemLibrary.h"

bool QTreeNode::AABBInclusion(const ENodeQuadrant QuadrantToCheck,TWeakObjectPtr<AActor> Object, const FVector& QueryPoint) const
{
	//Calculate the center of the node bounds
	FVector NodeCenter = (NodeData.Bounds.Min + NodeData.Bounds.Max) * 0.5f;
	
	//Use object bounds to calculate center
	FVector ActorCenter, ActorBoxExtent;
	
	if(Object.IsValid())
	{
		Object->GetActorBounds(false, ActorCenter, ActorBoxExtent);
	} else
	{
		ActorCenter = QueryPoint;
	}
	
	switch (QuadrantToCheck)
	{
	case ENodeQuadrant::NW:
		return (ActorCenter.X < NodeCenter.X && ActorCenter.Y > NodeCenter.Y);

	case ENodeQuadrant::NE:
		return (ActorCenter.X >= NodeCenter.X && ActorCenter.Y > NodeCenter.Y);

	case ENodeQuadrant::SW:
		return (ActorCenter.X < NodeCenter.X && ActorCenter.Y <= NodeCenter.Y);

	case ENodeQuadrant::SE:
		return (ActorCenter.X >= NodeCenter.X && ActorCenter.Y <= NodeCenter.Y);

	default:
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, *FString::Printf(TEXT("Invalid Quadrant!")));
		return false;
	}
}

void QTreeNode::Insertion(TWeakObjectPtr<AActor> Object)
{
	//Check if this node is a leaf node
	if (NodeData.bIsLeaf)
	{

		//Insert the actor into the leaf node
		NodeData.Objects.Add(Object);

		//Check if the node is eligible for subdiv
		if (NodeData.Objects.Num() > MaxPerZone && NodeData.Level < TreeDepth)
		{
			Partition();
		}
	}
	else
	{
		//If this is not a leaf node, check child nodes
		for (int32 i = 0; i < 4; i++)
		{
			if (ChildNodes[i] && AABBInclusion(static_cast<ENodeQuadrant>(i), Object, FVector::ZeroVector))
			{
				ChildNodes[i]->Insertion(Object);
				break;
			}
		}
	}
}


void QTreeNode::Partition()
{
    //We dont want to subdivide if we are at the max depth
    if (NodeData.Level >= TreeDepth)
    {
    	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("At Max Depth!"));
        return;
    }
    //Calculate the dimensions of the child quadrants based on current bounds
    FVector ParentMin = NodeData.Bounds.Min;
    FVector ParentMax = NodeData.Bounds.Max;
    FVector ParentMidPoint = (ParentMin + ParentMax) * 0.5f;

    //Create child nodes for the quadrants
    for (int32 i = 0; i < 4; i++)
    {
	    ENodeQuadrant ChildQuadrant = static_cast<ENodeQuadrant>(i);
	    FBounds ChildBounds;

	    switch (ChildQuadrant)
	    {
	    case ENodeQuadrant::NW:
		    ChildBounds.Min = FVector(ParentMin.X, ParentMidPoint.Y, 0);
		    ChildBounds.Max = FVector(ParentMidPoint.X, ParentMax.Y,0);
		    break;

	    case ENodeQuadrant::NE:
		    ChildBounds.Min = ParentMidPoint;
		    ChildBounds.Max = ParentMax;
		    break;

	    case ENodeQuadrant::SW:
		    ChildBounds.Min = ParentMin;
		    ChildBounds.Max = ParentMidPoint;
		    break;

	    case ENodeQuadrant::SE:
		    ChildBounds.Min = FVector(ParentMidPoint.X, ParentMin.Y ,0);
		    ChildBounds.Max = FVector(ParentMax.X, ParentMidPoint.Y, 0);
		    break;

	    default:
		    break;
	    }
        	
	    ChildNodes[i] = MakeShared<QTreeNode>(RootNode, NodeData.Level + 1, ChildBounds, ChildQuadrant, MaxPerZone, TreeDepth, bFilterEmptyQuads);
    }

    //Move objects from the current node to child nodes since we're subdividing
    NodeData.bIsLeaf = false;
    for (TWeakObjectPtr<AActor> Object : NodeData.Objects)
    {
	    for (int32 i = 0; i < 4; i++)
	    {
		    if (AABBInclusion(static_cast<ENodeQuadrant>(i), Object, FVector::ZeroVector))
		    {
			    ChildNodes[i]->Insertion(Object);
			    break;
		    }
	    }
    }

    //Clear objects in the current node since theyve been moved
    NodeData.Objects.Empty();
}


TArray<TWeakObjectPtr<AActor>> QTreeNode::RangeQuery(const FVector& QueryPoint, float Radius, TArray<QTreeNode*>& ObjectNode)
{
	//All Queried Objects to be returned in an array
	TArray<TWeakObjectPtr<AActor>> Result;
	//Check if the current node is a leaf node (Root will return false)
	if (NodeData.bIsLeaf)
	{
		TArray<QTreeNode*> ResultNodes;
		
		for (TWeakObjectPtr<AActor> Object : NodeData.Objects)
		{
			const FVector ObjectLocation = Object->GetActorLocation();
			if (FMath::Abs(ObjectLocation.X - QueryPoint.X) <= Radius &&
				FMath::Abs(ObjectLocation.Y - QueryPoint.Y) <= Radius &&
				FMath::Abs(ObjectLocation.Z - QueryPoint.Z) <= Radius)
			{
				if(!ResultNodes.Contains(this))
				{
					ResultNodes.Add(this);
				}
				Result.Add(Object);
			}
		}
		ObjectNode.Append(ResultNodes); 
	}
	else
	{
		{
			//If this is not a leaf node, recursively check child nodes
			for (int32 i = 0; i < 4; ++i)
			{
				if (ChildNodes[i] && (AABBInclusion(static_cast<ENodeQuadrant>(i), nullptr, QueryPoint) ||
					AABBInclusion(static_cast<ENodeQuadrant>(i), nullptr, QueryPoint + FVector(Radius,0 ,0)) ||
					AABBInclusion(static_cast<ENodeQuadrant>(i), nullptr, QueryPoint + FVector(0, Radius, 0)) ||
					AABBInclusion(static_cast<ENodeQuadrant>(i), nullptr, QueryPoint + FVector(-Radius,0 ,0)) ||
					AABBInclusion(static_cast<ENodeQuadrant>(i), nullptr, QueryPoint + FVector(0, -Radius, 0))))
				{
					//Recursively call range query on child nodes
					Result.Append(ChildNodes[i]->RangeQuery(QueryPoint, Radius, ObjectNode));
				}
			}
		}
	}
	return Result;
}


void QTreeNode::DrawBounds(UWorld* World, float Time, float Thickness)
{
	if (!World)
	{
		return;
	}

	FVector Center = (NodeData.Bounds.Min + NodeData.Bounds.Max) * 0.5f;
	FVector Extend = (NodeData.Bounds.Max - NodeData.Bounds.Min) * 0.5f;
	FLinearColor DrawColor = NodeData.bIsWithinPlayerRange ? FLinearColor::Green : FLinearColor::Red;

	Center.Z = NodeData.bIsWithinPlayerRange ? Center.Z + 8 : Center.Z + 5;

	if(bFilterEmptyQuads)
	{
		if(NodeData.Objects.Num() != 0 && NodeData.Level != 0)
		{
			UKismetSystemLibrary::DrawDebugBox(World, Center, Extend + FVector(0, 0, 1), DrawColor, FRotator::ZeroRotator, Time, Thickness);
		}
	} else
	{
		UKismetSystemLibrary::DrawDebugBox(World, Center, Extend + FVector(0, 0, 1), DrawColor, FRotator::ZeroRotator, Time, Thickness);
	}
	
	for (int32 i = 0; i < 4; i++)
	{
		if (ChildNodes[i])
		{
			ChildNodes[i]->DrawBounds(World, Time, Thickness);
		}
	}
}

