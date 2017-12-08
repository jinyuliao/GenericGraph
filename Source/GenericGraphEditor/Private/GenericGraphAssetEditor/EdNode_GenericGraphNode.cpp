#include "EdNode_GenericGraphNode.h"

#define LOCTEXT_NAMESPACE "EdNode_GenericGraph"

UEdNode_GenericGraphNode::UEdNode_GenericGraphNode()
{
	bCanRenameNode = true;
}

UEdNode_GenericGraphNode::~UEdNode_GenericGraphNode()
{

}

void UEdNode_GenericGraphNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, "MultipleNodes", FString(), nullptr, TEXT("In"));
	CreatePin(EGPD_Output, "MultipleNodes", FString(), nullptr, TEXT("Out"));
}

UEdGraph_GenericGraph* UEdNode_GenericGraphNode::GetGenericGraphEdGraph()
{
	return Cast<UEdGraph_GenericGraph>(GetGraph());
}

FText UEdNode_GenericGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (GenericGraphNode == nullptr)
	{
		return Super::GetNodeTitle(TitleType);
	}
	else
	{
		return GenericGraphNode->GetNodeTitle();
	}
}

void UEdNode_GenericGraphNode::SetGenericGraphNode(UGenericGraphNode* InNode)
{
	GenericGraphNode = InNode;
}

FLinearColor UEdNode_GenericGraphNode::GetBackgroundColor() const
{
	return GenericGraphNode == nullptr ? FLinearColor::Black : GenericGraphNode->GetBackgroundColor();
}

UEdGraphPin* UEdNode_GenericGraphNode::GetInputPin() const
{
	return Pins[0];
}

UEdGraphPin* UEdNode_GenericGraphNode::GetOutputPin() const
{
	return Pins[1];
}

void UEdNode_GenericGraphNode::PostEditUndo()
{
	UEdGraphNode::PostEditUndo();
}

#undef LOCTEXT_NAMESPACE
