#pragma once

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
    FDialogueResultData ResultData;

public:
    UFUNCTION(BlueprintCallable)
    FDialogueResultData GetResultData() const { return ResultData; }
    
#if WITH_EDITOR
    virtual FLinearColor GetBackgroundColor() const override;
#endif
};
