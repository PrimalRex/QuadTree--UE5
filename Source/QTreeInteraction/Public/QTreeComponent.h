// ©2023 Developed by Harris Barra.

#pragma once

#include "CoreMinimal.h"
#include "QTreeManager.h"
#include "Components/ActorComponent.h"
#include "QTreeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QTREEINTERACTION_API UQTreeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQTreeComponent();

	//Manager (Example content only shows the use of 1 manager but multiple managers usage is in mind)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuadTree")
	TArray<AQTreeManager*> QTreeManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuadTree")
	float QueryRadius = 100.f;
	
	UFUNCTION(BlueprintCallable)
	void RegisterWithQTreeManager();

	UFUNCTION(BlueprintCallable)
	void QueryActorsInRadius();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	


		
};
