#include "GenericGraphNode.h"

#define LOCTEXT_NAMESPACE "GenericGraphNode"

UGenericGraphNode::UGenericGraphNode()
{
	NodeType = AActor::StaticClass();
	BackgroundColor = FLinearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

UGenericGraphNode::~UGenericGraphNode()
{

}

FString UGenericGraphNode::GetNodeTitle()
{
	if (CustomNodeTitle.IsEmpty())
	{
		UClass* C = *NodeType;

		FString Title = C->GetName();
		Title.RemoveFromEnd("_C");

		return Title;
	}
	else
	{
		return CustomNodeTitle;
	}
}

UGenericGraph* UGenericGraphNode::GetGraph()
{
	return Cast<UGenericGraph>(GetOuter());
}

#undef LOCTEXT_NAMESPACE
