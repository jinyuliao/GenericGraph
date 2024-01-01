#pragma once

#include "CoreMinimal.h"
#include "GenericGraph.h"
#include "DialogueGraph.generated.h"

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
};