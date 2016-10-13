#include "GenericGraphEditorPrivatePCH.h"
#include "GenericGraphEdNode.h"

#define LOCTEXT_NAMESPACE "GenericGraphEdNode"

void UGenericGraphEdNode::SetGenericGraphNode(UGenericGraphNode* InNode)
{
	GenericGraphNode = InNode;
}

FText UGenericGraphEdNode::GetDescription() const
{
	UGenericGraph* Graph = GenericGraphNode->GetGraph();

	FString StoredClassName = Graph->NodeType->GetClass()->GetName();
	StoredClassName.RemoveFromEnd(TEXT("_C"));

	return FText::Format(LOCTEXT("NodeClassError", "Class {0} not found, make sure it's saved!"), FText::FromString(StoredClassName));
}

#undef LOCTEXT_NAMESPACE
