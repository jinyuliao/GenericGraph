#include "DialogueGraph/DialogueGraphNode.h"
#include "DialogueGraph/DialogueGraph.h"

#define LOCTEXT_NAMESPACE "DialogueGraphNode"

#if WITH_EDITORONLY_DATA

UDialogueGraphNode::UDialogueGraphNode()
{
    CompatibleGraphType = UDialogueGraph::StaticClass();

    DefaultNodeTitle = LOCTEXT("DialogueText", "DialogueText");
}

#endif

#if WITH_EDITOR

TArray<UDialogueGraphNode*> UDialogueGraphNode::GetChildrenNodes() const
{
    TArray<UDialogueGraphNode*> DialogueNodes;

    for (const auto& Node : ChildrenNodes)
    {
        if (const auto& DialogueNode = Cast<UDialogueGraphNode>(Node))
        {
            DialogueNodes.Add(DialogueNode);
        }
    }
    return DialogueNodes;
}

FText UDialogueGraphNode::GetNodeTitle() const
{
    return VisualData.DialogueText.IsEmpty() ? DefaultNodeTitle : VisualData.DialogueText;

}

void UDialogueGraphNode::SetNodeTitle(const FText& NewTitle)
{
    VisualData.DialogueText = NodeTitle;
}

FLinearColor UDialogueGraphNode::GetBackgroundColor() const
{
    return FLinearColor::Black;
}

#endif

#undef LOCTEXT_NAMESPACE