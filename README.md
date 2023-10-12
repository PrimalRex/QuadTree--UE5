# QuadTree--UE5
 A simple QuadTree Algorithm w/  searching
 
![quadgidv2](https://github.com/PrimalRex/QuadTree--UE5/assets/23298834/cce3b1d2-7430-4041-8aba-b5258e45e50a)
![quadgifv3](https://github.com/PrimalRex/QuadTree--UE5/assets/23298834/411261da-538c-4ac1-a00c-239688760a61)

This is a small example of a Tree-based data structure implementation. QuadTree Algorithm + Unreal Spice ontop for visuals.

# USAGE
*QTreeNode*
- Main Node for recursive based tree generation.
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
