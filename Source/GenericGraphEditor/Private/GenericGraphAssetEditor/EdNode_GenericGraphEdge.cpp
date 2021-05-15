#include "GenericGraphAssetEditor/EdNode_GenericGraphEdge.h"
#include "GenericGraphEdge.h"
#include "GenericGraphAssetEditor/EdNode_GenericGraphNode.h"

#define LOCTEXT_NAMESPACE "EdNode_GenericGraphEdge"

UEdNode_GenericGraphEdge::UEdNode_GenericGraphEdge()
{
	bCanRenameNode = true;
}

void UEdNode_GenericGraphEdge::SetEdge(UGenericGraphEdge* Edge)
{
	GenericGraphEdge = Edge;
}

void UEdNode_GenericGraphEdge::AllocateDefaultPins()
{
	UEdGraphPin* Inputs = CreatePin(EGPD_Input, TEXT("Edge"), FName(), TEXT("In"));
	Inputs->bHidden = true;
	UEdGraphPin* Outputs = CreatePin(EGPD_Output, TEXT("Edge"), FName(), TEXT("Out"));
	Outputs->bHidden = true;
}

FText UEdNode_GenericGraphEdge::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (GenericGraphEdge)
	{
		return GenericGraphEdge->GetNodeTitle();
	}
	return FText();
}

void UEdNode_GenericGraphEdge::PinConnectionListChanged(UEdGraphPin* Pin)
{
	if (Pin->LinkedTo.Num() == 0)
	{
		// Commit suicide; transitions must always have an input and output connection
		Modify();

		// Our parent graph will have our graph in SubGraphs so needs to be modified to record that.
		if (UEdGraph* ParentGraph = GetGraph())
		{
			ParentGraph->Modify();
		}

		DestroyNode();
	}
}

void UEdNode_GenericGraphEdge::PrepareForCopying()
{
	GenericGraphEdge->Rename(nullptr, this, REN_DontCreateRedirectors | REN_DoNotDirty);
}

void UEdNode_GenericGraphEdge::CreateConnections(UEdNode_GenericGraphNode* Start, UEdNode_GenericGraphNode* End)
{
	Pins[0]->Modify();
	Pins[0]->LinkedTo.Empty();

	Start->GetOutputPin()->Modify();
	Pins[0]->MakeLinkTo(Start->GetOutputPin());

	// This to next
	Pins[1]->Modify();
	Pins[1]->LinkedTo.Empty();

	End->GetInputPin()->Modify();
	Pins[1]->MakeLinkTo(End->GetInputPin());
}

UEdNode_GenericGraphNode* UEdNode_GenericGraphEdge::GetStartNode()
{
	if (Pins[0]->LinkedTo.Num() > 0)
	{
		return Cast<UEdNode_GenericGraphNode>(Pins[0]->LinkedTo[0]->GetOwningNode());
	}
	else
	{
		return nullptr;
	}
}

UEdNode_GenericGraphNode* UEdNode_GenericGraphEdge::GetEndNode()
{
	if (Pins[1]->LinkedTo.Num() > 0)
	{
		return Cast<UEdNode_GenericGraphNode>(Pins[1]->LinkedTo[0]->GetOwningNode());
	}
	else
	{
		return nullptr;
	}
}

#undef LOCTEXT_NAMESPACE

