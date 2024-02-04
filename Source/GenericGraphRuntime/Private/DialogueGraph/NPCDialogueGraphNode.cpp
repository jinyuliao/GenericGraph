#include "DialogueGraph/NPCDialogueGraphNode.h"
#include "DialogueGraph/DialogueGraph.h"

#define LOCTEXT_NAMESPACE "DialogueGraphNode"

#if WITH_EDITORONLY_DATA

UNPCDialogueGraphNode::UNPCDialogueGraphNode()
{
    CompatibleGraphType = UDialogueGraph::StaticClass();

    ContextMenuName = LOCTEXT("ContextMenuName", "NPC Dialogue Graph Node");

    DefaultNodeTitle = LOCTEXT("NPCDialogueText", "NPCDialogueText");
}

FLinearColor UNPCDialogueGraphNode::GetBackgroundColor() const
{
    
    if (const UDialogueGraph* DialogueGraph = Cast<UDialogueGraph>(GetGraph()))
    {
        return DialogueGraph->NPCDialogueNodeColor;
    }

    return Super::GetBackgroundColor();
}

#endif