#include "GenericGraph.h"
#include "GenericGraphRuntimePrivatePCH.h"

#define LOCTEXT_NAMESPACE "GenericGraph"

UGenericGraph::UGenericGraph()
{
	NodeType = UGenericGraphNode::StaticClass();

#if WITH_EDITORONLY_DATA
	EdGraph = nullptr;
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

			FString Message = FString::Printf(TEXT("%s, Level %d"), *Node->GetNodeTitle(), Level);

			if (ToConsole)
			{
				LOG_WARNING(TEXT("%s"), *Message);
			}

			if (ToScreen && GEngine != nullptr)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, Message);
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

int UGenericGraph::GetLevelNum()
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

void UGenericGraph::ClearGraph()
{
	for (int i = 0; i < AllNodes.Num(); ++i)
	{
		UGenericGraphNode* Node = AllNodes[i];

		Node->ParentNodes.Reset();
		Node->ChildrenNodes.Reset();
	}

	AllNodes.Reset();
	RootNodes.Reset();
}

#undef LOCTEXT_NAMESPACE
