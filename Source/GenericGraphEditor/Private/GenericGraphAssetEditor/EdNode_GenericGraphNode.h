#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "EdNode_GenericGraphNode.generated.h"

class UGenericGraphNode;
class UEdNode_GenericGraphEdge;

UCLASS(MinimalAPI)
class UEdNode_GenericGraphNode : public UEdGraphNode
{
	GENERATED_BODY()

public:
	UEdNode_GenericGraphNode();
	virtual ~UEdNode_GenericGraphNode();

	UPROPERTY(VisibleAnywhere, instanced, Category = "GenericGraph")
	UGenericGraphNode* GenericGraphNode;

	virtual void AllocateDefaultPins() override;

	UEdGraph_GenericGraph* GetGenericGraphEdGraph();

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;

	void SetGenericGraphNode(UGenericGraphNode* InNode);

	virtual FLinearColor GetBackgroundColor() const;

	virtual UEdGraphPin* GetInputPin() const;
	virtual UEdGraphPin* GetOutputPin() const;

#if WITH_EDITOR
	virtual void PostEditUndo() override;
#endif

};
