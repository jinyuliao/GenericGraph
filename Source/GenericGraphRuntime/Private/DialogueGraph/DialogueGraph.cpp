#include "DialogueGraph/DialogueGraph.h"
#include "DialogueGraph/DialogueGraphNode.h"
#include "DialogueGraph/DialogueGraphEdge.h"

#define LOCTEXT_NAMESPACE "DialogueGraph"

UDialogueGraph::UDialogueGraph()
{
    NodeType = UDialogueGraphNode::StaticClass();
    EdgeType = UDialogueGraphEdge::StaticClass();

    NPCDialogueNodeColor = FLinearColor::Red;
    PlayerDialogueNodeColor = FLinearColor::Green;
    
    Name = "DialogueGraph";
}

#undef LOCTEXT_NAMESPACE