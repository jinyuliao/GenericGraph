#include "GenericGraph.h"
#include "GenericGraphRuntimePCH.h"
#include "Containers/Deque.h"
#include "Engine/Engine.h"

#define LOCTEXT_NAMESPACE "GenericGraph"

UGenericGraph::UGenericGraph()
{
	NodeType = UGenericGraphNode::StaticClass();
	EdgeType = UGenericGraphEdge::StaticClass();

	bEdgeEnabled = true;

#if WITH_EDITORONLY_DATA
	EdGraph = nullptr;

	bCanRenameNode = true;
#endif
}

UGenericGraph::~UGenericGraph()
{

}

void UGenericGraph::Print(bool ToConsole /*= true*/, bool ToScreen /*= true*/)
{
	int Level = 0;
	TArray<UGenericGraphNode*> CurrLevelNodes = RootNodes;
	TArray<UGenericGraphNode*> NextLevelNodes;

	while (CurrLevelNodes.Num() != 0)
	{
		for (int i = 0; i < CurrLevelNodes.Num(); ++i)
		{
			UGenericGraphNode* Node = CurrLevelNodes[i];
			check(Node != nullptr);

			FString Message = FString::Printf(TEXT("%s, Level %d"), *Node->GetDescription().ToString(), Level);

			if (ToConsole)
			{
				LOG_INFO(TEXT("%s"), *Message);
			}

			if (ToScreen && GEngine != nullptr)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, Message);
			}

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

int UGenericGraph::GetLevelNum() const
{
	int Level = 0;
	TArray<UGenericGraphNode*> CurrLevelNodes = RootNodes;
	TArray<UGenericGraphNode*> NextLevelNodes;

	while (CurrLevelNodes.Num() != 0)
	{
		for (int i = 0; i < CurrLevelNodes.Num(); ++i)
		{
			UGenericGraphNode* Node = CurrLevelNodes[i];
			check(Node != nullptr);

			for (int j = 0; j < Node->ChildrenNodes.Num(); ++j)
			{
				NextLevelNodes.Add(Node->ChildrenNodes[j]);
			}
		}

		CurrLevelNodes = NextLevelNodes;
		NextLevelNodes.Reset();
		++Level;
	}

	return Level;
}

void UGenericGraph::GetNodesByLevel(int Level, TArray<UGenericGraphNode*>& Nodes)
{
	int CurrLEvel = 0;
	TArray<UGenericGraphNode*> NextLevelNodes;

	Nodes = RootNodes;

	while (Nodes.Num() != 0)
	{
		if (CurrLEvel == Level)
			break;

		for (int i = 0; i < Nodes.Num(); ++i)
		{
			UGenericGraphNode* Node = Nodes[i];
			check(Node != nullptr);

			for (int j = 0; j < Node->ChildrenNodes.Num(); ++j)
			{
				NextLevelNodes.Add(Node->ChildrenNodes[j]);
			}
		}

		Nodes = NextLevelNodes;
		NextLevelNodes.Reset();
		++CurrLEvel;
	}
}

UGenericGraphNode* UGenericGraph::GetNodeByID(int32 ID) const
{
    // Create a set to store visited nodes
    TSet<UGenericGraphNode*> VisitedNodes;

    // Create a queue for breadth-first search
    TQueue<UGenericGraphNode*> NodeQueue;

    // Enqueue the root nodes to start the search
    for (UGenericGraphNode* RootNode : RootNodes)
    {
        NodeQueue.Enqueue(RootNode);
        VisitedNodes.Add(RootNode);
    }

    // Perform breadth-first search
    while (!NodeQueue.IsEmpty())
    {
        UGenericGraphNode* CurrentNode;
        NodeQueue.Dequeue(CurrentNode);

        // Check if the current node has the desired ID
        if (CurrentNode->ID == ID)
        {
            return CurrentNode; // Found the node with the specified ID
        }

        // Enqueue unvisited neighbors
        for (UGenericGraphNode* Neighbor : CurrentNode->ChildrenNodes)
        {
            if (!VisitedNodes.Contains(Neighbor))
            {
                NodeQueue.Enqueue(Neighbor);
                VisitedNodes.Add(Neighbor);
            }
        }
    }

    // Node with the specified ID not found
    return nullptr;
}

UGenericGraphNode* UGenericGraph::GetFirstNode() const
{
    if (RootNodes.Num() > 0)
    {
        return RootNodes[0];
    }

    // Handle the case where RootNodes is empty (no nodes in the graph)
    return nullptr;
}

void UGenericGraph::ClearGraph()
{
	for (int i = 0; i < AllNodes.Num(); ++i)
	{
		UGenericGraphNode* Node = AllNodes[i];
		if (Node)
		{
			Node->ParentNodes.Empty();
			Node->ChildrenNodes.Empty();
			Node->Edges.Empty();
		}
	}

	AllNodes.Empty();
	RootNodes.Empty();
}



#undef LOCTEXT_NAMESPACE
