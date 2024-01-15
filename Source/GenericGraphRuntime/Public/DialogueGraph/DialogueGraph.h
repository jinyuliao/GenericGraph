#pragma once

#include "CoreMinimal.h"
#include "DialogueGraphNode.h"
#include "GenericGraph.h"
#include "DialogueGraph.generated.h"

class UDialogueGraphNode;
class UNPCDialogueGraphNode;
class UPlayerDialogueGraphNode;

UCLASS(Blueprintable)
class GENERICGRAPHRUNTIME_API UDialogueGraph : public UGenericGraph
{
    GENERATED_BODY()

public:
    UDialogueGraph();

    UPROPERTY(EditDefaultsOnly, Category = "DialogueGraph")
    FLinearColor NPCDialogueNodeColor;

    UPROPERTY(EditDefaultsOnly, Category = "DialogueGraph")
    FLinearColor PlayerDialogueNodeColor;

private:
    FGameplayTagContainer ValidationTags;

public:
    void InitializeValidationTags(const FGameplayTagContainer& GamePlayTags) { ValidationTags == GamePlayTags; }
    UNPCDialogueGraphNode* GetStartDialogueNode() const;
    UNPCDialogueGraphNode* GetNPCDialogueNode(const UPlayerDialogueGraphNode* SourceNode) const;
    TArray<UPlayerDialogueGraphNode*> GetPlayerDialogueNodes(const UNPCDialogueGraphNode* SourceNode) const;
    
private:
    bool ValidateNodeByGamePlayTag(const UDialogueGraphNode* Node, const FGameplayTagContainer& GamePlayTags, bool bEmptyTagsNodeIsValid) const;
    UDialogueGraphNode* GetValidRootNode(const TArray<UGenericGraphNode*>& RootNodes, const FGameplayTagContainer& GamePlayTags) const;
    TArray<UDialogueGraphNode*> GetValidNodes(const TArray<UDialogueGraphNode*>& SourceNodes, const FGameplayTagContainer& GamePlayTags) const;
};