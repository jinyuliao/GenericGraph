#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "GenericGraphEdNode.generated.h"

class UGenericGraphNode;

UCLASS(MinimalAPI)
class UGenericGraphEdNode : public UEdGraphNode
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, instanced, Category = "GenericGraph")
	UGenericGraphNode* GenericGraphNode;

	virtual void AllocateDefaultPins() override;
	virtual void NodeConnectionListChanged() override;

	UGenericGraphEdGraph* GetGenericGraphEdGraph();

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const;

	void SetGenericGraphNode(UGenericGraphNode* InNode);

	virtual FText GetDescription() const;

	virtual FLinearColor GetBackgroundColor() const;
};
