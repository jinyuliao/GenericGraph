#include "GenericGraphEditorPrivatePCH.h"
#include "GenericGraphAssetGraphSchema.h"
#include "GenericGraphEdNode.h"

#define LOCTEXT_NAMESPACE "GenericGraphAssetSchema"

UEdGraphNode* FGenericGraphAssetSchemaAction_NewNode::PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode /*= true*/)
{
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

	check(Graph != nullptr);

	ContextMenuBuilder.AddAction(NewNodeAction);
}

void UGenericGraphAssetGraphSchema::GetContextMenuActions(const UEdGraph* CurrentGraph, const UEdGraphNode* InGraphNode, const UEdGraphPin* InGraphPin, class FMenuBuilder* MenuBuilder, bool bIsDebugging) const
{
	if (InGraphPin)
	{
		MenuBuilder->BeginSection("GenericGraphAssetGraphSchemaNodeActions", LOCTEXT("PinActionsMenuHeader", "Pin Actions"));
		{
			// Only display the 'Break Link' option if there is a link to break!
			if (InGraphPin->LinkedTo.Num() > 0)
			{
				MenuBuilder->AddMenuEntry(FGraphEditorCommands::Get().BreakPinLinks);
			}
		}
		MenuBuilder->EndSection();
	}
	else if(InGraphNode != nullptr)
	{
		MenuBuilder->BeginSection("GenericGraphAssetGraphSchemaNodeActions", LOCTEXT("NodeActionsMenuHeader", "Node Actions"));
		{
			MenuBuilder->AddMenuEntry(FGraphEditorCommands::Get().BreakNodeLinks);
		}
		MenuBuilder->EndSection();
	}

	Super::GetContextMenuActions(CurrentGraph, InGraphNode, InGraphPin, MenuBuilder, bIsDebugging);
}

#undef LOCTEXT_NAMESPACE
