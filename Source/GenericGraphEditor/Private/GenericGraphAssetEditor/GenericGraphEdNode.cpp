#include "GenericGraphEditorPrivatePCH.h"
#include "GenericGraphEdNode.h"
#include "GenericGraphEditorTypes.h"

#define LOCTEXT_NAMESPACE "GenericGraphEdNode"

void UGenericGraphEdNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, UGenericGraphEditorTypes::PinCategory_MultipleNodes, TEXT(""), NULL, false, false, TEXT("In"));
	CreatePin(EGPD_Output, UGenericGraphEditorTypes::PinCategory_MultipleNodes, TEXT(""), NULL, false, false, TEXT("Out"));
}

void UGenericGraphEdNode::NodeConnectionListChanged()
{
	Super::NodeConnectionListChanged();

	GetGenericGraphEdGraph()->RebuildGenericGraph();
}

UGenericGraphEdGraph* UGenericGraphEdNode::GetGenericGraphEdGraph()
{
	return Cast<UGenericGraphEdGraph>(GetGraph());
}

FText UGenericGraphEdNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (GenericGraphNode == nullptr)
	{
		return Super::GetNodeTitle(TitleType);
	}
	else
	{
		UClass* C = *GenericGraphNode->NodeType;
		return FText::FromString(C->GetName());
	}
}

void UGenericGraphEdNode::SetGenericGraphNode(UGenericGraphNode* InNode)
{
	GenericGraphNode = InNode;
}

FText UGenericGraphEdNode::GetDescription() const
{
	UGenericGraph* Graph = GenericGraphNode->GetGraph();

	UClass* C = *GenericGraphNode->NodeType;

	return FText::FromString(C->GetDescription());
}

#undef LOCTEXT_NAMESPACE
