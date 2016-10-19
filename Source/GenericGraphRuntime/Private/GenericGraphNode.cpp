#include "GenericGraphRuntimePrivatePCH.h"
#include "GenericGraphNode.h"

#define LOCTEXT_NAMESPACE "GenericGraphNode"

UGenericGraphNode::UGenericGraphNode()
{
	NodeType = AActor::StaticClass();
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
