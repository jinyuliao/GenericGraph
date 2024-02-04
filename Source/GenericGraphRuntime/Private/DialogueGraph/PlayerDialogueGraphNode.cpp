#include "DialogueGraph/PlayerDialogueGraphNode.h"
#include "DialogueGraph/DialogueGraph.h"

#define LOCTEXT_NAMESPACE "DialogueGraphNode"

#if WITH_EDITORONLY_DATA

UPlayerDialogueGraphNode::UPlayerDialogueGraphNode()
{
    CompatibleGraphType = UDialogueGraph::StaticClass();

    ContextMenuName = LOCTEXT("ContextMenuName", "Player Dialogue Graph Node");

    DefaultNodeTitle = LOCTEXT("PlayerDialogueText", "PlayerDialogueText");
}

FLinearColor UPlayerDialogueGraphNode::GetBackgroundColor() const
{
    if (const UDialogueGraph* DialogueGraph = Cast<UDialogueGraph>(GetGraph()))
    {
        return DialogueGraph->PlayerDialogueNodeColor;
    }
    
    return Super::GetBackgroundColor();
}

#endif