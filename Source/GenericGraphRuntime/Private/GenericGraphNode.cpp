#include "GenericGraphNode.h"
#include "GenericGraph.h"

#define LOCTEXT_NAMESPACE "GenericGraphNode"

UGenericGraphNode::UGenericGraphNode()
{
#if WITH_EDITORONLY_DATA
	CompatibleGraphType = UGenericGraph::StaticClass();

	BackgroundColor = FLinearColor::Black;
#endif
}

UGenericGraphNode::~UGenericGraphNode()
{

}


FLinearColor UGenericGraphNode::GetBackgroundColor_Implementation() const
{
#if WITH_EDITORONLY_DATA
	return BackgroundColor;
#else
	return FLinearColor::Black;
#endif
}

UGenericGraphEdge* UGenericGraphNode::GetEdge(UGenericGraphNode* ChildNode)
{
	return Edges.Contains(ChildNode) ? Edges.FindChecked(ChildNode) : nullptr;
}

FText UGenericGraphNode::GetNodeTitle_Implementation() const
{
	if (NodeTitle.IsEmpty())
	{
		return LOCTEXT("DefaultNodeTitle", "Generic Graph Node");
	}
	else
	{
		return NodeTitle;
	}
}

void UGenericGraphNode::SetNodeTitle_Implementation(const FText& NewTitle)
{
	NodeTitle = NewTitle;
}

bool UGenericGraphNode::IsLeafNode() const
{
	return ChildrenNodes.Num() == 0;
}

UGenericGraph* UGenericGraphNode::GetGraph() const
{
	return Graph;
}

#undef LOCTEXT_NAMESPACE
