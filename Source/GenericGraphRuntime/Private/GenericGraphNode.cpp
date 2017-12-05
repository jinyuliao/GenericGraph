#include "GenericGraphNode.h"

#define LOCTEXT_NAMESPACE "GenericGraphNode"

UGenericGraphNode::UGenericGraphNode()
{
	NodeType = AActor::StaticClass();

#if WITH_EDITOR
	BackgroundColor = FLinearColor::Black;
#endif
}

UGenericGraphNode::~UGenericGraphNode()
{

}

FText UGenericGraphNode::GetNodeTitle_Implementation()
{
	if (CustomNodeTitle.IsEmpty())
	{
		UClass* C = *NodeType;

		FString Title = C->GetName();
		Title.RemoveFromEnd("_C");

		return FText::FromString(Title);
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

UGenericGraph* UGenericGraphNode::GetGraph()
{
	return Cast<UGenericGraph>(GetOuter());
}

#undef LOCTEXT_NAMESPACE
