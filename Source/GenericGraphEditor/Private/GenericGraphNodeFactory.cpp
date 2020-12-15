#include "GenericGraphNodeFactory.h"
#include <EdGraph/EdGraphNode.h>
#include "GenericGraphAssetEditor/SEdNode_GenericGraphEdge.h"
#include "GenericGraphAssetEditor/EdNode_GenericGraphNode.h"
#include "GenericGraphAssetEditor/SEdNode_GenericGraphNode.h"
#include "GenericGraphAssetEditor/EdNode_GenericGraphEdge.h"

TSharedPtr<class SGraphNode> FGraphPanelNodeFactory_GenericGraph::CreateNode(UEdGraphNode* Node) const
{
	if (UEdNode_GenericGraphNode* EdNode_GraphNode = Cast<UEdNode_GenericGraphNode>(Node))
	{
		return SNew(SEdNode_GenericGraphNode, EdNode_GraphNode);
	}
	else if (UEdNode_GenericGraphEdge* EdNode_Edge = Cast<UEdNode_GenericGraphEdge>(Node))
	{
		return SNew(SEdNode_GenericGraphEdge, EdNode_Edge);
	}
	return nullptr;
}

