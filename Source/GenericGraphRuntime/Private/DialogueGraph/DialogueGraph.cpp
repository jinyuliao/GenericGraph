#include "DialogueGraph/DialogueGraph.h"
#include "DialogueGraph/DialogueGraphNode.h"
#include "DialogueGraph/DialogueGraphEdge.h"
#include "DialogueGraph/DialogueGraphNode.h"
#include "DialogueGraph/NPCDialogueGraphNode.h"
#include "DialogueGraph/PlayerDialogueGraphNode.h"
#define LOCTEXT_NAMESPACE "DialogueGraph"

UDialogueGraph::UDialogueGraph()
{
    NodeType = UDialogueGraphNode::StaticClass();
    EdgeType = UDialogueGraphEdge::StaticClass();

    NPCDialogueNodeColor = FLinearColor::Red;
    PlayerDialogueNodeColor = FLinearColor::Green;
    
    Name = "DialogueGraph";
}

UNPCDialogueGraphNode* UDialogueGraph::GetStartDialogueNode() const
{
    return Cast<UNPCDialogueGraphNode>(GetValidRootNode(RootNodes, ValidationTags));
}

UNPCDialogueGraphNode* UDialogueGraph::GetNPCDialogueNode(const UPlayerDialogueGraphNode* SourceNode) const
{
    const auto ChildrenNodes = SourceNode->ChildrenNodes;
    if (ChildrenNodes.IsEmpty()) return nullptr;
    
    return Cast<UNPCDialogueGraphNode>(ChildrenNodes[0]);
}

TArray<UPlayerDialogueGraphNode*> UDialogueGraph::GetPlayerDialogueNodes(const UNPCDialogueGraphNode* SourceNode) const
{
    GetValidNodes(SourceNode->ChildrenNodes, ValidationTags);
}

bool UDialogueGraph::ValidateNodeByGamePlayTag(const UDialogueGraphNode* Node, const FGameplayTagContainer& GamePlayTags, bool bEmptyTagsNodeIsValid) const
{
    const auto NodeConditionTags = Node->GetConditionData().ConditionTags;
    
    if (bEmptyTagsNodeIsValid)
    {
        if (NodeConditionTags.Num() == 0) return true;
    }

    if (GamePlayTags.HasAll(NodeConditionTags))
    {
        return true;
    }
    return false;
}

UDialogueGraphNode* UDialogueGraph::GetValidRootNode(const TArray<UGenericGraphNode*>& RootNodes,
    const FGameplayTagContainer& GamePlayTags) const
{
    
    for (const auto Node : RootNodes)
    {
        const auto DialogueNode = Cast<UDialogueGraphNode>(Node);
        if (!DialogueNode) break;
        
        const auto bIsValid = ValidateNodeByGamePlayTag(DialogueNode, GamePlayTags, false);

        if (bIsValid) return DialogueNode;
    }

    return nullptr;
}

TArray<UDialogueGraphNode*> UDialogueGraph::GetValidNodes(const TArray<UDialogueGraphNode*>& SourceNodes,
    const FGameplayTagContainer& GamePlayTags) const
{
    TArray<UDialogueGraphNode*> ValidNodes;

    for (const auto Node : SourceNodes)
    {
        const auto bIsValid = ValidateNodeByGamePlayTag(Node, GamePlayTags, true);

        if (bIsValid) ValidNodes.Add(Node);
    }
    
    return ValidNodes;
}

#undef LOCTEXT_NAMESPACE