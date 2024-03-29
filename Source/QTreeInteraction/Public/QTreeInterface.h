// ©2023 Developed by Harris Barra.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "QTreeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UQTreeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class QTREEINTERACTION_API IQTreeInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="QuadTree")
	void OnQueriedBySearch();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="QuadTree")
	void OnUnqueriedBySearch();
};
