#pragma once

#include "CoreMinimal.h"
#include "GenericGraphNode.generated.h"

class UGenericGraph;

UCLASS(Blueprintable)
class GENERICGRAPHRUNTIME_API UGenericGraphNode : public UObject
{
	GENERATED_BODY()

public:
	UGenericGraphNode();
	virtual ~UGenericGraphNode();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GenericGraphNode")
	TSubclassOf<UObject> NodeType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GenericGraphNode")
	FText CustomNodeTitle;

	UPROPERTY(BlueprintReadOnly, Category = "GenericGraphNode")
	TArray<UGenericGraphNode*> ParentNodes;

	UPROPERTY(BlueprintReadOnly, Category = "GenericGraphNode")
	TArray<UGenericGraphNode*> ChildrenNodes;

#if WITH_EDITOR
	UPROPERTY(EditDefaultsOnly, Category = "GenericGraphNode")
	FLinearColor BackgroundColor;

	UPROPERTY(EditDefaultsOnly, Category = "GenericGraphNode")
	FText ContextMenuName;
#endif

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "GenericGraphNode")
	void EnterNode(AActor* OwnerActor);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "GenericGraphNode")
	void ExitNode(AActor* OwnerActor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GenericGraphNode")
	FText GetNodeTitle();
	FText GetNodeTitle_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GenericGraphNode")
	void SetCustomNodeTitle(const FText& NewTitle);
	virtual void SetCustomNodeTitle_Implementation(const FText& NewTitle);

	UGenericGraph* GetGraph();
};
