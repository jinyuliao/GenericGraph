#pragma once

#include "GenericGraphEdNode.generated.h"

UCLASS(MinimalAPI)
class UGenericGraphEdNode : public UEdGraphNode
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, instanced, Category = "GenericGraph")
	UGenericGraphNode* GenericGraphNode;

	void SetGenericGraphNode(UGenericGraphNode* InNode);
};