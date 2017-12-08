#include "GenericGraphNode.h"

#define LOCTEXT_NAMESPACE "GenericGraphNode"

UGenericGraphNode::UGenericGraphNode()
{
	CompatibleGraphType = UGenericGraph::StaticClass();

#if WITH_EDITOR
	BackgroundColor = FLinearColor::Black;
#endif
}

UGenericGraphNode::~UGenericGraphNode()
{

}


FLinearColor UGenericGraphNode::GetBackgroundColor_Implementation() const
{
#if WITH_EDITOR
	return BackgroundColor;
#else
	return FLinearColor::Black;
#endif
}

UGenericGraphEdge* UGenericGraphNode::GetEdge(UGenericGraphNode* ChildNode)
{
	return Edges.Contains(ChildNode) ? Edges.FindChecked(ChildNode) : nullptr;
}

FText UGenericGraphNode::GetNodeTitle_Implementation()
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

UGenericGraph* UGenericGraphNode::GetGraph() const
{
	return Graph;
}

#undef LOCTEXT_NAMESPACE
