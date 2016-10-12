#include "GenericGraphEditorPrivatePCH.h"
#include "GenericGraphAssetGraphSchema.h"
#include "GenericGraphEdNode.h"

#define LOCTEXT_NAMESPACE "GenericGraphAssetSchema"

UEdGraphNode* FGenericGraphAssetSchemaAction_NewNode::PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode /*= true*/)
{
	check(NodeData);

	UGenericGraph* Graph = CastChecked<UGenericGraph>(ParentGraph->GetOuter());

	const FScopedTransaction Transaction(LOCTEXT("GenericGraphEditorNewNode", "Generic Graph Editor: New Node"));
	ParentGraph->Modify();
	Graph->Modify();

	UGenericGraphNode* NewNode = NewObject<UGenericGraphNode>(Graph, UGenericGraphNode::StaticClass());

	Graph->AllNodes.Add(NewNode);

	FGraphNodeCreator<UGenericGraphEdNode> NodeCreator(*Graph->EdGraph);
	UGenericGraphEdNode* GraphNode = NodeCreator.CreateNode(true);
	GraphNode->SetGenericGraphNode(NewNode);
	NodeCreator.Finalize();

	// If this node allows >0 children but by default has zero - create a connector for starters
// 	if (NewNode->GetMaxChildNodes() > 0 && NewNode->ChildNodes.Num() == 0)
// 	{
// 		NewNode->CreateStartingConnectors();
// 	}
// 
// 	// Attempt to connect inputs to selected nodes, unless we're already dragging from a single output
// 	if (FromPin == NULL || FromPin->Direction == EGPD_Input)
// 	{
// 		ConnectToSelectedNodes(NewNode, ParentGraph);
// 	}

	GraphNode->NodePosX = Location.X;
	GraphNode->NodePosY = Location.Y;

	GraphNode->AutowireNewNode(FromPin);

	Graph->PostEditChange();
	Graph->MarkPackageDirty();

	return GraphNode;
}

void UGenericGraphAssetGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	const bool bNoParent = (ContextMenuBuilder.FromPin == NULL);

	const FText AddToolTip = LOCTEXT("NewGenericGraphNodeTooltip", "Add node here");
	const FText Desc = LOCTEXT("NewGenericGraphNodeTooltip", "Add Node");
	TSharedPtr<FGenericGraphAssetSchemaAction_NewNode> NewNodeAction(new FGenericGraphAssetSchemaAction_NewNode(LOCTEXT("GenericGraphNodeAction", "Generic Graph Node"), Desc, AddToolTip.ToString(), 0));

	UGenericGraph* Graph = Cast<UGenericGraph>(GetOuter());
	if (Graph != nullptr)
	{
		NewNodeAction->NodeData = Graph->NodeType;
	}
	else
	{
		NewNodeAction->NodeData = UObject::StaticClass();
		LOG_WARNING(TEXT("UGenericGraphAssetGraphSchema::GetGraphContextActions outer is null"));
	}

	ContextMenuBuilder.AddAction(NewNodeAction);
}

#undef LOCTEXT_NAMESPACE
