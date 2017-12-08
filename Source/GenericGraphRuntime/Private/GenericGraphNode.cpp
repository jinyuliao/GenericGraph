#include "GenericGraphNode.h"

#define LOCTEXT_NAMESPACE "GenericGraphNode"

UGenericGraphNode::UGenericGraphNode()
{
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
	if (CustomNodeTitle.IsEmpty())
	{
		return LOCTEXT("DefaultNodeTitle", "Generic Graph Node");
	}
	else
	{
		return CustomNodeTitle;
	}
}

void UGenericGraphNode::SetCustomNodeTitle_Implementation(const FText& NewTitle)
{
	CustomNodeTitle = NewTitle;
}

UGenericGraph* UGenericGraphNode::GetGraph() const
{
	return Cast<UGenericGraph>(GetOuter());
}

#undef LOCTEXT_NAMESPACE
