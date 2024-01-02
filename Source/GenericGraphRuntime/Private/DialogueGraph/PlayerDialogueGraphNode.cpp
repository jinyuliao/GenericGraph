#include "DialogueGraph/PlayerDialogueGraphNode.h"
#include "DialogueGraph/DialogueGraph.h"

#define LOCTEXT_NAMESPACE "DialogueGraphNode"

#if WITH_EDITORONLY_DATA

UPlayerDialogueGraphNode::UPlayerDialogueGraphNode()
{
    CompatibleGraphType = UDialogueGraph::StaticClass();
    DialogueNodeType = EDialogueNodeType::Player;
    ContextMenuName = LOCTEXT("ContextMenuName", "Player Dialogue Graph Node");

    DefaultNodeTitle = LOCTEXT("PlayerDialogueText", "PlayerDialogueText");
}

#endif