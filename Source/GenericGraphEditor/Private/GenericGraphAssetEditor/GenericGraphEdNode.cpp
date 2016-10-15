#include "GenericGraphEditorPrivatePCH.h"
#include "GenericGraphEdNode.h"
#include "GenericGraphEditorTypes.h"

#define LOCTEXT_NAMESPACE "GenericGraphEdNode"

void UGenericGraphEdNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, UGenericGraphEditorTypes::PinCategory_MultipleNodes, TEXT(""), NULL, false, false, TEXT("In"));
	CreatePin(EGPD_Output, UGenericGraphEditorTypes::PinCategory_MultipleNodes, TEXT(""), NULL, false, false, TEXT("Out"));
}

void UGenericGraphEdNode::SetGenericGraphNode(UGenericGraphNode* InNode)
{
	GenericGraphNode = InNode;
}

FText UGenericGraphEdNode::GetDescription() const
{
	UGenericGraph* Graph = GenericGraphNode->GetGraph();

	return FText::FromString(Graph->NodeType->GetClass()->GetDescription());

// 	FString StoredClassName = Graph->NodeType->GetClass()->GetName();
// 	StoredClassName.RemoveFromEnd(TEXT("_C"));
// 
// 	return FText::FromString(StoredClassName);
}

#undef LOCTEXT_NAMESPACE
