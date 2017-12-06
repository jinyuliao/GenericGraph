#pragma once

#include "CoreMinimal.h"
#include "GenericGraphNode.h"
#include "GenericGraphEdge.generated.h"

class UGenericGraph;

UCLASS(Blueprintable)
class GENERICGRAPHRUNTIME_API UGenericGraphEdge : public UObject
{
	GENERATED_BODY()

public:
	UGenericGraphEdge();
	virtual ~UGenericGraphEdge();

	UPROPERTY(BlueprintReadOnly, Category = "GenericGraphTransition")
	UGenericGraphNode* StartNode;

	UPROPERTY(BlueprintReadOnly, Category = "GenericGraphTransition")
	UGenericGraphNode* EndNode;

	UFUNCTION(BlueprintPure, Category = "GenericGraphEdge")
	UGenericGraph* GetGraph() const;
};
