#pragma once

#include "GenericGraphEdNode.generated.h"

UCLASS(MinimalAPI)
class UGenericGraphEdNode : public UEdGraphNode
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, instanced, Category = "GenericGraph")
	UGenericGraphNode* GenericGraphNode;

	virtual void AllocateDefaultPins() override;

	void SetGenericGraphNode(UGenericGraphNode* InNode);

	virtual FText GetDescription() const;
};