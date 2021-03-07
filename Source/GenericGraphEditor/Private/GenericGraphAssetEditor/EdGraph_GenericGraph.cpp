#include "GenericGraphAssetEditor/EdGraph_GenericGraph.h"
#include "GenericGraphEditorPCH.h"
#include "GenericGraph.h"
#include "GenericGraphAssetEditor/EdNode_GenericGraphNode.h"
#include "GenericGraphAssetEditor/EdNode_GenericGraphEdge.h"

UEdGraph_GenericGraph::UEdGraph_GenericGraph()
{

}

UEdGraph_GenericGraph::~UEdGraph_GenericGraph()
{

}

void UEdGraph_GenericGraph::RebuildGenericGraph()
{
	LOG_INFO(TEXT("UGenericGraphEdGraph::RebuildGenericGraph has been called"));

	UGenericGraph* Graph = GetGenericGraph();

	Clear();

	for (int i = 0; i < Nodes.Num(); ++i)
	{
		if (UEdNode_GenericGraphNode* EdNode = Cast<UEdNode_GenericGraphNode>(Nodes[i]))
		{
			if (EdNode->GenericGraphNode == nullptr)
				continue;

			UGenericGraphNode* GenericGraphNode = EdNode->GenericGraphNode;

			NodeMap.Add(GenericGraphNode, EdNode);

			Graph->AllNodes.Add(GenericGraphNode);

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
						UEdNode_GenericGraphNode* Child = EdNode_Edge->GetEndNode();;
						if (Child != nullptr)
						{
							ChildNode = Child->GenericGraphNode;
						}
					}

					if (ChildNode != nullptr)
					{
						GenericGraphNode->ChildrenNodes.Add(ChildNode);

						ChildNode->ParentNodes.Add(GenericGraphNode);
					}
					else
					{
						LOG_ERROR(TEXT("UEdGraph_GenericGraph::RebuildGenericGraph can't find child node"));
					}
				}
			}
		}
		else if (UEdNode_GenericGraphEdge* EdgeNode = Cast<UEdNode_GenericGraphEdge>(Nodes[i]))
		{
			UEdNode_GenericGraphNode* StartNode = EdgeNode->GetStartNode();
			UEdNode_GenericGraphNode* EndNode = EdgeNode->GetEndNode();
			UGenericGraphEdge* Edge = EdgeNode->GenericGraphEdge;

			if (StartNode == nullptr || EndNode == nullptr || Edge == nullptr)
			{
				LOG_ERROR(TEXT("UEdGraph_GenericGraph::RebuildGenericGraph add edge failed."));
				continue;
			}

			EdgeMap.Add(Edge, EdgeNode);

			Edge->Graph = Graph;
			Edge->Rename(nullptr, Graph, REN_DontCreateRedirectors | REN_DoNotDirty);
			Edge->StartNode = StartNode->GenericGraphNode;
			Edge->EndNode = EndNode->GenericGraphNode;
			Edge->StartNode->Edges.Add(Edge->EndNode, Edge);
		}
	}

	for (int i = 0; i < Graph->AllNodes.Num(); ++i)
	{
		UGenericGraphNode* Node = Graph->AllNodes[i];
		if (Node->ParentNodes.Num() == 0)
		{
			Graph->RootNodes.Add(Node);

			SortNodes(Node);
		}

		Node->Graph = Graph;
		Node->Rename(nullptr, Graph, REN_DontCreateRedirectors | REN_DoNotDirty);
	}

	Graph->RootNodes.Sort([&](const UGenericGraphNode& L, const UGenericGraphNode& R)
	{
		UEdNode_GenericGraphNode* EdNode_LNode = NodeMap[&L];
		UEdNode_GenericGraphNode* EdNode_RNode = NodeMap[&R];
		return EdNode_LNode->NodePosX < EdNode_RNode->NodePosX;
	});
}

UGenericGraph* UEdGraph_GenericGraph::GetGenericGraph() const
{
	return CastChecked<UGenericGraph>(GetOuter());
}

bool UEdGraph_GenericGraph::Modify(bool bAlwaysMarkDirty /*= true*/)
{
	bool Rtn = Super::Modify(bAlwaysMarkDirty);

	GetGenericGraph()->Modify();

	for (int32 i = 0; i < Nodes.Num(); ++i)
	{
		Nodes[i]->Modify();
	}

	return Rtn;
}

void UEdGraph_GenericGraph::Clear()
{
	UGenericGraph* Graph = GetGenericGraph();

	Graph->ClearGraph();
	NodeMap.Reset();
	EdgeMap.Reset();

	for (int i = 0; i < Nodes.Num(); ++i)
	{
		if (UEdNode_GenericGraphNode* EdNode = Cast<UEdNode_GenericGraphNode>(Nodes[i]))
		{
			UGenericGraphNode* GenericGraphNode = EdNode->GenericGraphNode;
			GenericGraphNode->ParentNodes.Reset();
			GenericGraphNode->ChildrenNodes.Reset();
			GenericGraphNode->Edges.Reset();
		}
	}
}

void UEdGraph_GenericGraph::SortNodes(UGenericGraphNode* RootNode)
{
	int Level = 0;
	TArray<UGenericGraphNode*> CurrLevelNodes = { RootNode };
	TArray<UGenericGraphNode*> NextLevelNodes;

	while (CurrLevelNodes.Num() != 0)
	{
		int32 LevelWidth = 0;
		for (int i = 0; i < CurrLevelNodes.Num(); ++i)
		{
			UGenericGraphNode* Node = CurrLevelNodes[i];

			auto Comp = [&](const UGenericGraphNode& L, const UGenericGraphNode& R)
			{
				UEdNode_GenericGraphNode* EdNode_LNode = NodeMap[&L];
				UEdNode_GenericGraphNode* EdNode_RNode = NodeMap[&R];
				return EdNode_LNode->NodePosX < EdNode_RNode->NodePosX;
			};

			Node->ChildrenNodes.Sort(Comp);
			Node->ParentNodes.Sort(Comp);

			for (int j = 0; j < Node->ChildrenNodes.Num(); ++j)
			{
				NextLevelNodes.Add(Node->ChildrenNodes[j]);
			}
		}

		CurrLevelNodes = NextLevelNodes;
		NextLevelNodes.Reset();
		++Level;
	}
}

void UEdGraph_GenericGraph::PostEditUndo()
{
	Super::PostEditUndo();

	NotifyGraphChanged();
}

