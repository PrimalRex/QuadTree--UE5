# QuadTree--UE5
 A simple QuadTree Algorithm w/  searching
 
![quadpreview](https://github.com/PrimalRex/QuadTree--UE5/assets/23298834/89fe0ad1-6035-40f5-9cbf-c4504d917b4c)

This is a small example of a Tree-based data structure implementation. QuadTree Algorithm + Unreal Spice ontop for visuals.

# USAGE
*QTreeNode*
- Main Node for recurssive based tree generation.
- Features AABBInclusion, Insertion, Partition, Experimental RangeQuery

*QTreeManager*
- Creates and manages an instance of a QuadTree
- Adjustable in scale, quadrant depth + size

*QTreeItem*
- Child Actor that is spawned and is queried in each quandrant
- Uses Interface for Debug

*QTreeComponent*
- Component that lives on the pawn or player
- Drives the QTreeManager
- Adjustable radius

*QTreeInterface*
- Underlying Interface to communicate between QNodes and QItems


*!!!!Built on UE5.1, Source is backwards compat but not example content!!!!*
