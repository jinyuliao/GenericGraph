#pragma once

#include "CoreMinimal.h"
#include "GenericGraphNode.h"
#include "DialogueStructures.h"
#include "DialogueGraphNode.generated.h"

UCLASS(Blueprintable)
class GENERICGRAPHRUNTIME_API UDialogueGraphNode : public UGenericGraphNode
{
    GENERATED_BODY()
    
public:
    UDialogueGraphNode();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DialogueGraph")
    FDialogueVisualData VisualData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DialogueGraph")
    FDialogueConditionData ConditionData;
    
    
    FText DefaultNodeTitle;
    
public:
    UFUNCTION(BlueprintCallable)
    FDialogueVisualData GetVisualData() const { return VisualData; }

    UFUNCTION(BlueprintCallable)
    FDialogueConditionData GetConditionData() const { return ConditionData; }

    UFUNCTION(BlueprintCallable)
    TArray<UDialogueGraphNode*> GetChildrenNodes() const;
    
#if WITH_EDITOR
    virtual FText GetNodeTitle() const override;

    virtual void SetNodeTitle(const FText& NewTitle) override;

    virtual FLinearColor GetBackgroundColor() const override;
#endif
};
