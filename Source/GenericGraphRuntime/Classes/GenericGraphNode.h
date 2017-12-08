#pragma once

#include "CoreMinimal.h"
#include "GenericGraphNode.generated.h"

class UGenericGraph;
class UGenericGraphEdge;

UCLASS(Blueprintable)
class GENERICGRAPHRUNTIME_API UGenericGraphNode : public UObject
{
	GENERATED_BODY()

public:
	UGenericGraphNode();
	virtual ~UGenericGraphNode();

	UPROPERTY(VisibleDefaultsOnly, Category = "GenericGraphNode")
	UGenericGraph* Graph;

	UPROPERTY(VisibleDefaultsOnly, Category = "GenericGraphNode")
	TSubclassOf<UGenericGraph> CompatibleGraphType;

	UPROPERTY(EditDefaultsOnly, Category = "GenericGraphNode")
	FText NodeTitle;

	UPROPERTY(BlueprintReadOnly, Category = "GenericGraphNode")
	TArray<UGenericGraphNode*> ParentNodes;

	UPROPERTY(BlueprintReadOnly, Category = "GenericGraphNode")
	TArray<UGenericGraphNode*> ChildrenNodes;

	UPROPERTY(BlueprintReadOnly, Category = "GenericGraphNode")
	TMap<UGenericGraphNode*, UGenericGraphEdge*> Edges;

#if WITH_EDITOR
	UPROPERTY(EditDefaultsOnly, Category = "GenericGraphNode")
	FLinearColor BackgroundColor;

	UPROPERTY(EditDefaultsOnly, Category = "GenericGraphNode")
	FText ContextMenuName;
#endif

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GenericGraphNode")
	FLinearColor GetBackgroundColor() const;
	virtual FLinearColor GetBackgroundColor_Implementation() const;

	UFUNCTION(BlueprintCallable, Category = "GenericGraphNode")
	virtual UGenericGraphEdge* GetEdge(UGenericGraphNode* ChildNode);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GenericGraphNode")
	FText GetNodeTitle();
	virtual FText GetNodeTitle_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GenericGraphNode")
	void SetNodeTitle(const FText& NewTitle);
	virtual void SetNodeTitle_Implementation(const FText& NewTitle);

	UFUNCTION(BlueprintCallable, Category = "GenericGraphNode")
	UGenericGraph* GetGraph() const;
};
