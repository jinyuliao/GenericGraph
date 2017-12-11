#include "AutoLayout/AutoLayoutStrategy.h"
#include "Kismet/KismetMathLibrary.h"
#include "GenericGraphAssetEditor/EdNode_GenericGraphNode.h"
#include "GenericGraphAssetEditor/SEdNode_GenericGraphNode.h"

UAutoLayoutStrategy::UAutoLayoutStrategy()
{
	Settings = nullptr;
	MaxIteration = 50;
	OptimalDistance = 150;
}

UAutoLayoutStrategy::~UAutoLayoutStrategy()
{

}

FBox2D UAutoLayoutStrategy::GetNodeBound(UEdGraphNode* EdNode)
{
	int32 NodeWidth = GetNodeWidth(Cast<UEdNode_GenericGraphNode>(EdNode));
	int32 NodeHeight = GetNodeHeight(Cast<UEdNode_GenericGraphNode>(EdNode));
	FVector2D Min(EdNode->NodePosX, EdNode->NodePosY);
	FVector2D Max(EdNode->NodePosX + NodeWidth, EdNode->NodePosY + NodeHeight);
	return FBox2D(Min, Max);
}

FBox2D UAutoLayoutStrategy::GetActualBounds(UGenericGraphNode* RootNode)
{
	int Level = 0;
	TArray<UGenericGraphNode*> CurrLevelNodes = { RootNode };
	TArray<UGenericGraphNode*> NextLevelNodes;

	FBox2D Rtn = GetNodeBound(EdGraph->NodeMap[RootNode]);

	while (CurrLevelNodes.Num() != 0)
	{
		for (int i = 0; i < CurrLevelNodes.Num(); ++i)
		{
			UGenericGraphNode* Node = CurrLevelNodes[i];
			check(Node != nullptr);

			Rtn += GetNodeBound(EdGraph->NodeMap[Node]);

			for (int j = 0; j < Node->ChildrenNodes.Num(); ++j)
			{
				NextLevelNodes.Add(Node->ChildrenNodes[j]);
			}
		}

		CurrLevelNodes = NextLevelNodes;
		NextLevelNodes.Reset();
		++Level;
	}
	return Rtn;
}

void UAutoLayoutStrategy::RandomLayoutOneTree(UGenericGraphNode* RootNode, const FBox2D& Bound)
{
	int Level = 0;
	TArray<UGenericGraphNode*> CurrLevelNodes = { RootNode };
	TArray<UGenericGraphNode*> NextLevelNodes;

	while (CurrLevelNodes.Num() != 0)
	{
		for (int i = 0; i < CurrLevelNodes.Num(); ++i)
		{
			UGenericGraphNode* Node = CurrLevelNodes[i];
			check(Node != nullptr);

			UEdNode_GenericGraphNode* EdNode_Node = EdGraph->NodeMap[Node];

			EdNode_Node->NodePosX = UKismetMathLibrary::RandomFloatInRange(Bound.Min.X, Bound.Max.X);
			EdNode_Node->NodePosY = UKismetMathLibrary::RandomFloatInRange(Bound.Min.Y, Bound.Max.Y);

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

int32 UAutoLayoutStrategy::GetNodeWidth(UEdNode_GenericGraphNode* EdNode)
{
	return EdNode->SEdNode->GetCachedGeometry().GetLocalSize().X;
}

int32 UAutoLayoutStrategy::GetNodeHeight(UEdNode_GenericGraphNode* EdNode)
{
	return EdNode->SEdNode->GetCachedGeometry().GetLocalSize().Y;
}

