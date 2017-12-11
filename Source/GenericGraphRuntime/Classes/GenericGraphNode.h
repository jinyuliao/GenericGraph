#pragma once

#include "CoreMinimal.h"
#include "SubclassOf.h"
#include "TextProperty.h"
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

	UPROPERTY(EditDefaultsOnly, Category = "GenericGraphNode")
	FText NodeTitle;

	UPROPERTY(BlueprintReadOnly, Category = "GenericGraphNode")
	TArray<UGenericGraphNode*> ParentNodes;

	UPROPERTY(BlueprintReadOnly, Category = "GenericGraphNode")
	TArray<UGenericGraphNode*> ChildrenNodes;

	UPROPERTY(BlueprintReadOnly, Category = "GenericGraphNode")
	TMap<UGenericGraphNode*, UGenericGraphEdge*> Edges;

#if WITH_EDITOR
	UPROPERTY(VisibleDefaultsOnly, Category = "GenericGraphNode")
	TSubclassOf<UGenericGraph> CompatibleGraphType;

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

	UFUNCTION(BlueprintCallable, Category = "GenericGraphNode")
	bool IsLeafNode() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GenericGraphNode")
	FText GetNodeTitle() const;
	virtual FText GetNodeTitle_Implementation() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GenericGraphNode")
	void SetNodeTitle(const FText& NewTitle);
	virtual void SetNodeTitle_Implementation(const FText& NewTitle);

	UFUNCTION(BlueprintCallable, Category = "GenericGraphNode")
	UGenericGraph* GetGraph() const;
};
