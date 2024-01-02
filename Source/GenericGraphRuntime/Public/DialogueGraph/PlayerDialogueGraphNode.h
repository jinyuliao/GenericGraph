﻿#pragma once

#include "CoreMinimal.h"
#include "DialogueGraphNode.h"
#include "PlayerDialogueGraphNode.generated.h"


UCLASS(Blueprintable)
class GENERICGRAPHRUNTIME_API UPlayerDialogueGraphNode : public UDialogueGraphNode
{
    GENERATED_BODY()
    
public:
    UPlayerDialogueGraphNode();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DialogueGraph")
    FDialogueConditionData ConditionData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DialogueGraph")
    FDialogueResultData ResultData;
};