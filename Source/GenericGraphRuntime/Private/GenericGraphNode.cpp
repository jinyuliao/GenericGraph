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

UGenericGraphEdge* UGenericGraphNode::GetEdge(UGenericGraphNode* ChildNode)
{
	return Edges.Contains(ChildNode) ? Edges.FindChecked(ChildNode) : nullptr;
}

FText UGenericGraphNode::GetDescription_Implementation() const
{
	return LOCTEXT("NodeDesc", "Generic Graph Node");
}

#if WITH_EDITOR
bool UGenericGraphNode::IsNameEditable() const
{
	return true;
}

FLinearColor UGenericGraphNode::GetBackgroundColor() const
{
	return BackgroundColor;
}

FText UGenericGraphNode::GetNodeTitle() const
{
	return NodeTitle.IsEmpty() ? GetDescription() : NodeTitle;
}

void UGenericGraphNode::SetNodeTitle(const FText& NewTitle)
{
	NodeTitle = NewTitle;
}

bool UGenericGraphNode::CanCreateConnection(UGenericGraphNode* Other, FText& ErrorMessage)
{
	return true;
}

#endif

bool UGenericGraphNode::IsLeafNode() const
{
	return ChildrenNodes.Num() == 0;
}

UGenericGraph* UGenericGraphNode::GetGraph() const
{
	return Graph;
}

#undef LOCTEXT_NAMESPACE
