// ©2023 Developed by Harris Barra.


#include "QTreeItem.h"

// Sets default values
AQTreeItem::AQTreeItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

AQTreeItem::~AQTreeItem()
{
}

void AQTreeItem::OnQueriedBySearch_Implementation()
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("IMPLEMENTED!"));
	DebugShowColor();
}

void AQTreeItem::OnUnqueriedBySearch_Implementation()
{
	DebugHideColor();
}

// Called when the game starts or when spawned
void AQTreeItem::BeginPlay()
{
	Super::BeginPlay();
	
}

