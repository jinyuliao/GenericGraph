#include "DialogueGraph/DialogueGraphNode.h"
#include "DialogueGraph/DialogueGraph.h"

#define LOCTEXT_NAMESPACE "DialogueGraphNode"

#if WITH_EDITORONLY_DATA

UDialogueGraphNode::UDialogueGraphNode()
{
    CompatibleGraphType = UDialogueGraph::StaticClass();
    DialogueNodeType = EDialogueNodeType::None;
}

#endif

#if WITH_EDITOR

FText UDialogueGraphNode::GetNodeTitle() const
{
    return DialogueText.IsEmpty() ? LOCTEXT("DialogueText", "(DialogueText)") : DialogueText;

}

void UDialogueGraphNode::SetNodeTitle(const FText& NewTitle)
{
    DialogueText = NodeTitle;
}

FLinearColor UDialogueGraphNode::GetBackgroundColor() const
{
    UDialogueGraph* DialogueGraph = Cast<UDialogueGraph>(GetGraph());

    if (DialogueGraph == nullptr)
        return Super::GetBackgroundColor();

    switch (DialogueNodeType)
    {
        case EDialogueNodeType::NPC:
            return DialogueGraph->NPCDialogueNodeColor;
        case EDialogueNodeType::Player:
            return DialogueGraph->PlayerDialogueNodeColor;
        default:
            return FLinearColor::Black;
    }
}

#endif

#undef LOCTEXT_NAMESPACE