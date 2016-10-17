#pragma once

#include "GenericGraphNode.generated.h"

class UGenericGraph;

UCLASS(Blueprintable)
class GENERICGRAPHRUNTIME_API UGenericGraphNode : public UObject
{
	GENERATED_BODY()

public:
	UGenericGraphNode();
	virtual ~UGenericGraphNode();

	UPROPERTY(EditAnywhere, Category = "GenericGraphNode")
	TSubclassOf<UObject> NodeType;

	UGenericGraph* GetGraph();
};
