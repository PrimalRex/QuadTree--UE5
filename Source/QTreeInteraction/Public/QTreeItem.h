// ©2023 Developed by Harris Barra.

#pragma once

#include "CoreMinimal.h"
#include "QTreeInterface.h"
#include "GameFramework/Actor.h"
#include "QTreeItem.generated.h"

UCLASS()
class QTREEINTERACTION_API AQTreeItem : public AActor, public IQTreeInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQTreeItem();
	virtual ~AQTreeItem() override;
	virtual void OnQueriedBySearch_Implementation() override;
	virtual void OnUnqueriedBySearch_Implementation() override;
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void DebugShowColor();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void DebugHideColor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	


};
