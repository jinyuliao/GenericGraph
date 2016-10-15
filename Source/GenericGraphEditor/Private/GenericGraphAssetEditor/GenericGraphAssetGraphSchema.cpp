#include "GenericGraphEditorPrivatePCH.h"
#include "GenericGraphAssetGraphSchema.h"
#include "GenericGraphEdNode.h"
#include "GenericGraphConnectionDrawingPolicy.h"

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

void UGenericGraphAssetGraphSchema::GetBreakLinkToSubMenuActions(class FMenuBuilder& MenuBuilder, class UEdGraphPin* InGraphPin)
{
	// Make sure we have a unique name for every entry in the list
	TMap< FString, uint32 > LinkTitleCount;

	// Add all the links we could break from
	for (TArray<class UEdGraphPin*>::TConstIterator Links(InGraphPin->LinkedTo); Links; ++Links)
	{
		UEdGraphPin* Pin = *Links;
		FString TitleString = Pin->GetOwningNode()->GetNodeTitle(ENodeTitleType::ListView).ToString();
		FText Title = FText::FromString(TitleString);
		if (Pin->PinName != TEXT(""))
		{
			TitleString = FString::Printf(TEXT("%s (%s)"), *TitleString, *Pin->PinName);

			// Add name of connection if possible
			FFormatNamedArguments Args;
			Args.Add(TEXT("NodeTitle"), Title);
			Args.Add(TEXT("PinName"), Pin->GetDisplayName());
			Title = FText::Format(LOCTEXT("BreakDescPin", "{NodeTitle} ({PinName})"), Args);
		}

		uint32 &Count = LinkTitleCount.FindOrAdd(TitleString);

		FText Description;
		FFormatNamedArguments Args;
		Args.Add(TEXT("NodeTitle"), Title);
		Args.Add(TEXT("NumberOfNodes"), Count);

		if (Count == 0)
		{
			Description = FText::Format(LOCTEXT("BreakDesc", "Break link to {NodeTitle}"), Args);
		}
		else
		{
			Description = FText::Format(LOCTEXT("BreakDescMulti", "Break link to {NodeTitle} ({NumberOfNodes})"), Args);
		}
		++Count;

		MenuBuilder.AddMenuEntry(Description, Description, FSlateIcon(), FUIAction(
			FExecuteAction::CreateUObject(this, &UGenericGraphAssetGraphSchema::BreakSinglePinLink, const_cast<UEdGraphPin*>(InGraphPin), *Links)));
	}
}

void UGenericGraphAssetGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	const bool bNoParent = (ContextMenuBuilder.FromPin == NULL);

	const FText AddToolTip = LOCTEXT("NewGenericGraphNodeTooltip", "Add node here");
	const FText Desc = LOCTEXT("NewGenericGraphNodeTooltip", "Add Node");
	TSharedPtr<FGenericGraphAssetSchemaAction_NewNode> NewNodeAction(new FGenericGraphAssetSchemaAction_NewNode(LOCTEXT("GenericGraphNodeAction", "Generic Graph Node"), Desc, AddToolTip.ToString(), 0));

	ContextMenuBuilder.AddAction(NewNodeAction);
}

void UGenericGraphAssetGraphSchema::GetContextMenuActions(const UEdGraph* CurrentGraph, const UEdGraphNode* InGraphNode, const UEdGraphPin* InGraphPin, class FMenuBuilder* MenuBuilder, bool bIsDebugging) const
{
	if (InGraphPin != nullptr)
	{
		MenuBuilder->BeginSection("GenericGraphAssetGraphSchemaNodeActions", LOCTEXT("PinActionsMenuHeader", "Pin Actions"));
		{
			// Only display the 'Break Link' option if there is a link to break!
			if (InGraphPin->LinkedTo.Num() > 0)
			{
				MenuBuilder->AddMenuEntry(FGraphEditorCommands::Get().BreakPinLinks);

				// add sub menu for break link to
				if (InGraphPin->LinkedTo.Num() > 1)
				{
					MenuBuilder->AddSubMenu(
						LOCTEXT("BreakLinkTo", "Break Link To..."),
						LOCTEXT("BreakSpecificLinks", "Break a specific link..."),
						FNewMenuDelegate::CreateUObject((UGenericGraphAssetGraphSchema*const)this, &UGenericGraphAssetGraphSchema::GetBreakLinkToSubMenuActions, const_cast<UEdGraphPin*>(InGraphPin)));
				}
				else
				{
					((UGenericGraphAssetGraphSchema*const)this)->GetBreakLinkToSubMenuActions(*MenuBuilder, const_cast<UEdGraphPin*>(InGraphPin));
				}
			}
		}
		MenuBuilder->EndSection();
	}
	else if(InGraphNode != nullptr)
	{
		MenuBuilder->BeginSection("GenericGraphAssetGraphSchemaNodeActions", LOCTEXT("NodeActionsMenuHeader", "Node Actions"));
		{
			MenuBuilder->AddMenuEntry(FGenericCommands::Get().Delete);
			MenuBuilder->AddMenuEntry(FGenericCommands::Get().Cut);
			MenuBuilder->AddMenuEntry(FGenericCommands::Get().Copy);
			MenuBuilder->AddMenuEntry(FGenericCommands::Get().Duplicate);

			MenuBuilder->AddMenuEntry(FGraphEditorCommands::Get().BreakNodeLinks);
		}
		MenuBuilder->EndSection();
	}

	Super::GetContextMenuActions(CurrentGraph, InGraphNode, InGraphPin, MenuBuilder, bIsDebugging);
}

const FPinConnectionResponse UGenericGraphAssetGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
	// Make sure the pins are not on the same node
	if (A->GetOwningNode() == B->GetOwningNode())
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("PinErrorSameNode", "Both are on the same node"));
	}

	return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, LOCTEXT("PinConnect", "Connect nodes"));
}

class FConnectionDrawingPolicy* UGenericGraphAssetGraphSchema::CreateConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float InZoomFactor, const FSlateRect& InClippingRect, class FSlateWindowElementList& InDrawElements, class UEdGraph* InGraphObj) const
{
	return new FGenericGraphConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, InZoomFactor, InClippingRect, InDrawElements, InGraphObj);
}

FLinearColor UGenericGraphAssetGraphSchema::GetPinTypeColor(const FEdGraphPinType& PinType) const
{
	return FColor::White;
}

void UGenericGraphAssetGraphSchema::BreakNodeLinks(UEdGraphNode& TargetNode) const
{
	const FScopedTransaction Transaction(NSLOCTEXT("UnrealEd", "GraphEd_BreakNodeLinks", "Break Node Links"));

	Super::BreakNodeLinks(TargetNode);
}

void UGenericGraphAssetGraphSchema::BreakPinLinks(UEdGraphPin& TargetPin, bool bSendsNodeNotifcation) const
{
	const FScopedTransaction Transaction(NSLOCTEXT("UnrealEd", "GraphEd_BreakPinLinks", "Break Pin Links"));

	Super::BreakPinLinks(TargetPin, bSendsNodeNotifcation);
}

void UGenericGraphAssetGraphSchema::BreakSinglePinLink(UEdGraphPin* SourcePin, UEdGraphPin* TargetPin)
{
	const FScopedTransaction Transaction(NSLOCTEXT("UnrealEd", "GraphEd_BreakSinglePinLink", "Break Pin Link"));

	Super::BreakSinglePinLink(SourcePin, TargetPin);
}

#undef LOCTEXT_NAMESPACE
