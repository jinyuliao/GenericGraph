#include "EdGraph_GenericGraph.h"
#include "EdNode_GenericGraphNode.h"
#include "EdNode_GenericGraphEdge.h"

UEdGraph_GenericGraph::UEdGraph_GenericGraph()
{

}

UEdGraph_GenericGraph::~UEdGraph_GenericGraph()
{

}

void UEdGraph_GenericGraph::RebuildGenericGraph()
{
	LOG_WARNING(TEXT("UGenericGraphEdGraph::RebuildGenericGraph has been called"));

	UGenericGraph* G = CastChecked<UGenericGraph>(GetOuter());

	G->ClearGraph();

	for (int i = 0; i < Nodes.Num(); ++i)
	{
		if (UEdNode_GenericGraphNode* EdNode = Cast<UEdNode_GenericGraphNode>(Nodes[i]))
		{
			if (EdNode->GenericGraphNode == nullptr)
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
					UGenericGraphNode* ChildNode = nullptr;
					if (UEdNode_GenericGraphNode* EdNode_Child = Cast<UEdNode_GenericGraphNode>(Pin->LinkedTo[LinkToIdx]->GetOwningNode()))
					{
						ChildNode = EdNode_Child->GenericGraphNode;
					}
					else if (UEdNode_GenericGraphEdge* EdNode_Edge = Cast<UEdNode_GenericGraphEdge>(Pin->LinkedTo[LinkToIdx]->GetOwningNode()))
					{
						UEdNode_GenericGraphNode* EdNode_Child = EdNode_Edge->GetEndNode();;
						if (EdNode_Child != nullptr)
						{
							ChildNode = EdNode_Child->GenericGraphNode;
						}
					}

					if (ChildNode != nullptr)
					{
						GNode->ChildrenNodes.Add(ChildNode);

						ChildNode->ParentNodes.Add(GNode);
					}
					else
					{
						LOG_ERROR(TEXT("UEdGraph_GenericGraph::RebuildGenericGraph can't find child node"));
					}
				}
			}
		}
		else if (UEdNode_GenericGraphEdge* TransitionNode = Cast<UEdNode_GenericGraphEdge>(Nodes[i]))
		{
			UEdNode_GenericGraphNode* StartNode = TransitionNode->GetStartNode();
			UEdNode_GenericGraphNode* EndNode = TransitionNode->GetEndNode();
			UGenericGraphEdge* Edge = TransitionNode->GenericGraphEdge;

			if (StartNode == nullptr || EndNode == nullptr || Edge == nullptr)
				continue;

			Edge->StartNode = StartNode->GenericGraphNode;
			Edge->EndNode = EndNode->GenericGraphNode;
			Edge->StartNode->Edges.Add(Edge->EndNode, Edge);
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
void UEdGraph_GenericGraph::PostEditUndo()
{
	Super::PostEditUndo();

	NotifyGraphChanged();
}
#endif

