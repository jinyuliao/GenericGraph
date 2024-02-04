#pragma once

#include "CoreMinimal.h"
#include "DialogueGraphNode.h"
#include "NPCDialogueGraphNode.generated.h"

UCLASS(Blueprintable)
class GENERICGRAPHRUNTIME_API UNPCDialogueGraphNode : public UDialogueGraphNode
{
    GENERATED_BODY()
    
public:
    UNPCDialogueGraphNode();

#if WITH_EDITOR
    virtual FLinearColor GetBackgroundColor() const override;
#endif
};
