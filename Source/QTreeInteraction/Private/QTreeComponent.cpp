// ©2023 Developed by Harris Barra.


#include "QTreeComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UQTreeComponent::UQTreeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UQTreeComponent::RegisterWithQTreeManager()
{
	QTreeManager.Empty();
	const TSubclassOf<AQTreeManager> ClassToFind = AQTreeManager::StaticClass();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, FoundActors);
	for (AActor* Actor : FoundActors)
	{
		QTreeManager.Add(Cast<AQTreeManager>(Actor));
	}
}

void UQTreeComponent::QueryActorsInRadius()
{
	if(QTreeManager.Num() == 0)
	{
		RegisterWithQTreeManager();
	}
	for (AQTreeManager* Manager : QTreeManager)
	{
		if(Manager)
		{
			Manager->QueryActorsInRadius(GetOwner()->GetActorLocation(), QueryRadius);
		}
	}
}

// Called when the game starts
void UQTreeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

