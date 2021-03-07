#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "EdNode_GenericGraphEdge.generated.h"

class UGenericGraphNode;
class UGenericGraphEdge;
class UEdNode_GenericGraphNode;

UCLASS(MinimalAPI)
class UEdNode_GenericGraphEdge : public UEdGraphNode
{
	GENERATED_BODY()

public:
	UPROPERTY()
	class UEdGraph* Graph;

	UPROPERTY(VisibleAnywhere, Instanced, Category = "GenericGraph")
	UGenericGraphEdge* GenericGraphEdge;

	void SetEdge(UGenericGraphEdge* Edge);

	virtual void AllocateDefaultPins() override;

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;

	virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;

	virtual void PrepareForCopying() override;

	virtual UEdGraphPin* GetInputPin() const { return Pins[0]; }
	virtual UEdGraphPin* GetOutputPin() const { return Pins[1]; }

	void CreateConnections(UEdNode_GenericGraphNode* Start, UEdNode_GenericGraphNode* End);

	UEdNode_GenericGraphNode* GetStartNode();
	UEdNode_GenericGraphNode* GetEndNode();
};
