#include "GenericGraphEditorPrivatePCH.h"
#include "GenericGraphEdGraph.h"
#include "GenericGraphEdNode.h"

UGenericGraphEdGraph::UGenericGraphEdGraph()
{

}

UGenericGraphEdGraph::~UGenericGraphEdGraph()
{

}

void UGenericGraphEdGraph::RebuildGenericGraph()
{
	LOG_WARNING(TEXT("UGenericGraphEdGraph::RebuildGenericGraph has been called"));

	UGenericGraph* G = CastChecked<UGenericGraph>(GetOuter());

	G->ClearGraph();

	for (int i = 0; i < Nodes.Num(); ++i)
	{
		UGenericGraphEdNode* EdNode = Cast<UGenericGraphEdNode>(Nodes[i]);

		if (EdNode == nullptr || EdNode->GenericGraphNode == nullptr)
			continue;

		UGenericGraphNode* GNode = EdNode->GenericGraphNode;

		G->AllNodes.Add(GNode);

		for (int PinIdx = 0; PinIdx < EdNode->Pins.Num(); ++PinIdx)
		{
			UEdGraphPin* Pin = EdNode->Pins[PinIdx];

			if (Pin->Direction != EEdGraphPinDirection::EGPD_Output)
				continue;

			for (int LinkToIdx = 0; LinkToIdx < Pin->LinkedTo.Num(); ++LinkToIdx)
			{
				UGenericGraphEdNode* ChildEdNode = Cast<UGenericGraphEdNode>(Pin->LinkedTo[LinkToIdx]->GetOwningNode());

				if (ChildEdNode == nullptr)
					continue;

				UGenericGraphNode* ChildNode = ChildEdNode->GenericGraphNode;

				GNode->ChildrenNodes.Add(ChildNode);

				ChildNode->ParentNodes.Add(GNode);
			}
		}
	}

	for (int i = 0; i < G->AllNodes.Num(); ++i)
	{
		UGenericGraphNode* Node = G->AllNodes[i];
		if (Node->ParentNodes.Num() == 0)
		{
			G->RootNodes.Add(Node);
		}
	}
}

#if WITH_EDITOR
void UGenericGraphEdGraph::PostEditUndo()
{
	Super::PostEditUndo();

	RebuildGenericGraph();

	Modify();
}
#endif

