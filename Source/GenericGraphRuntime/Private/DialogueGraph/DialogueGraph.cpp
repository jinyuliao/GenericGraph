#include "DialogueGraph/DialogueGraph.h"
#include "DialogueGraph/DialogueGraphNode.h"
#include "DialogueGraph/DialogueGraphEdge.h"
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

void UDialogueGraph::InitializeValidationTags(const FGameplayTagContainer& GamePlayTags)
{
    UE_LOG(LogTemp, Warning, TEXT("Initalize"));
    ValidationTags = GamePlayTags;
}

UNPCDialogueGraphNode* UDialogueGraph::GetStartDialogueNode() const
{
    return Cast<UNPCDialogueGraphNode>(GetValidRootNode(RootNodes, ValidationTags));
}

UNPCDialogueGraphNode* UDialogueGraph::GetNPCDialogueNode(const UPlayerDialogueGraphNode* SourceNode) const
{
    if (!SourceNode)
    {
        return nullptr; // Null check
    }

    const auto ChildrenNodes = SourceNode->ChildrenNodes;
    if (ChildrenNodes.IsEmpty())
    {
        return nullptr;
    }

    return Cast<UNPCDialogueGraphNode>(ChildrenNodes[0]);
}

TArray<UPlayerDialogueGraphNode*> UDialogueGraph::GetPlayerDialogueNodes(const UNPCDialogueGraphNode* SourceNode) const
{
    if (!SourceNode)
    {
        return TArray<UPlayerDialogueGraphNode*>(); // Null check and return empty array if null
    }

    TArray<UDialogueGraphNode*> ValidNodes = GetValidNodes(SourceNode->ChildrenNodes, ValidationTags);
    
    TArray<UPlayerDialogueGraphNode*> PlayerDialogueGraphNodes;

    for (const auto& ValidNode : ValidNodes)
    {
        if (const auto& PlayerDialogueNode = Cast<UPlayerDialogueGraphNode>(ValidNode))
        {
            PlayerDialogueGraphNodes.Add(PlayerDialogueNode);
        }
    }
    return PlayerDialogueGraphNodes;
}

bool UDialogueGraph::ValidateNodeByGamePlayTag(const UDialogueGraphNode* Node, const FGameplayTagContainer& GamePlayTags,
    bool bEmptyTagsNodeIsValid) const
{
    if (!Node)
    {
        return false; // Null check
    }

    const auto& NodeConditionTags = Node->GetConditionData().ConditionTags;

    if (bEmptyTagsNodeIsValid && NodeConditionTags.Num() == 0)
    {
        return true;
    }

    return GamePlayTags.HasAll(NodeConditionTags);
}

UDialogueGraphNode* UDialogueGraph::GetValidRootNode(const TArray<UGenericGraphNode*>& Nodes,
    const FGameplayTagContainer& GamePlayTags) const
{
    UE_LOG(LogTemp, Warning, TEXT("GetValidRootNode"));

    UDialogueGraphNode* ValidNode = nullptr;
    
    for (const auto& Node : Nodes) // Using const reference
    {        
        const auto DialogueNode = Cast<UDialogueGraphNode>(Node);
        if (!DialogueNode)
        {
            continue; // Null check
        }

        if (ValidateNodeByGamePlayTag(DialogueNode, GamePlayTags, false))
        {
            ValidNode =  DialogueNode;
        }
    }

    return ValidNode;
}

TArray<UDialogueGraphNode*> UDialogueGraph::GetValidNodes(const TArray<UGenericGraphNode*>& SourceNodes,
    const FGameplayTagContainer& GamePlayTags) const
{
    TArray<UDialogueGraphNode*> ValidNodes;

    for (const auto& Node : SourceNodes) // Using const reference
    {
        const auto DialogueNode = Cast<UDialogueGraphNode>(Node);
        
        if(!DialogueNode) continue;
        
        if (ValidateNodeByGamePlayTag(DialogueNode, GamePlayTags, true))
        {
            ValidNodes.Add(DialogueNode);
        }
    }

    return ValidNodes; // Return by value
}


#undef LOCTEXT_NAMESPACE