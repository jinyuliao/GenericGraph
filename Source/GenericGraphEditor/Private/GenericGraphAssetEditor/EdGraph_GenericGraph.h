#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"
#include "EdGraph_GenericGraph.generated.h"

class UGenericGraph;
class UGenericGraphNode;
class UGenericGraphEdge;
class UEdNode_GenericGraphNode;
class UEdNode_GenericGraphEdge;

UCLASS()
class UEdGraph_GenericGraph : public UEdGraph
{
	GENERATED_BODY()

public:
	UEdGraph_GenericGraph();
	virtual ~UEdGraph_GenericGraph();

	virtual void RebuildGenericGraph();

	UGenericGraph* GetGenericGraph() const;

#if WITH_EDITOR
	virtual void PostEditUndo() override;
#endif

	UPROPERTY(Transient)
	TMap<UGenericGraphNode*, UEdNode_GenericGraphNode*> NodeMap;

	UPROPERTY(Transient)
	TMap<UGenericGraphEdge*, UEdNode_GenericGraphEdge*> EdgeMap;
};
