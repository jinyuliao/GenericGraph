#pragma once

#include "CoreMinimal.h"
#include "GenericGraphNode.h"
#include "DialogueGraphNode.generated.h"

UENUM(BlueprintType)
enum class EDialogueNodeType : uint8
{
    None,
    NPC,
    Player
};

UCLASS(Blueprintable)
class GENERICGRAPHRUNTIME_API UDialogueGraphNode : public UGenericGraphNode
{
    GENERATED_BODY()
    
public:
    UDialogueGraphNode();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DialogueGraph")
    FText DialogueText;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DialogueGraph")
    EDialogueNodeType DialogueNodeType;

public:
    EDialogueNodeType GetDialogueNodeType() const { return DialogueNodeType; }
    
#if WITH_EDITOR
    virtual FText GetNodeTitle() const override;

    virtual void SetNodeTitle(const FText& NewTitle) override;

    virtual FLinearColor GetBackgroundColor() const override;
#endif
};
