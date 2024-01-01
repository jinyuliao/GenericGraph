#include "DialogueGraph/NPCDialogueGraphNode.h"
#include "DialogueGraph/DialogueGraph.h"

#define LOCTEXT_NAMESPACE "DialogueGraphNode"

#if WITH_EDITORONLY_DATA

UNPCDialogueGraphNode::UNPCDialogueGraphNode()
{
    CompatibleGraphType = UDialogueGraph::StaticClass();
    DialogueNodeType = EDialogueNodeType::NPC;
    ContextMenuName = LOCTEXT("ContextMenuName", "NPC Dialogue Graph Node");
}

#endif