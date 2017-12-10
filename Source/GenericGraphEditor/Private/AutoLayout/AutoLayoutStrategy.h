#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"
#include "GenericGraph.h"
#include "GenericGraphAssetEditor/EdGraph_GenericGraph.h"
#include "GenericGraphAssetEditor/EdNode_GenericGraphNode.h"
#include "GenericGraphAssetEditor/EdNode_GenericGraphEdge.h"
#include "AutoLayoutStrategy.generated.h"

UCLASS(abstract)
class UAutoLayoutStrategy : public UObject
{
	GENERATED_BODY()
public:
	UAutoLayoutStrategy();
	virtual ~UAutoLayoutStrategy();

	virtual void Layout(UEdGraph* EdGraph) {};

protected:
	virtual FBox2D GetNodeBound(UEdGraphNode* EdNode);

	virtual FBox2D GetActualBounds(UGenericGraphNode* RootNode);

	virtual void RandomLayoutOneTree(UGenericGraphNode* RootNode, const FBox2D& Bound);

protected:
	UGenericGraph * Graph;
	UEdGraph_GenericGraph* EdGraph;
	float OptimalDistance;
};
