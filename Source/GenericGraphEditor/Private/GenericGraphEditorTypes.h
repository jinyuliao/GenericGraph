#pragma once

#include "CoreMinimal.h"
#include "GenericGraphEditorTypes.generated.h"

UCLASS()
class UGenericGraphEditorTypes : public UObject
{
	GENERATED_UCLASS_BODY()

	static const FString PinCategory_MultipleNodes;
	static const FString PinCategory_SingleNode;
};
