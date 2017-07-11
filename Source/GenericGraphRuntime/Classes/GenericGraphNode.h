#pragma once

#include "GenericGraphNode.generated.h"

class UGenericGraph;

UCLASS(Blueprintable)
class GENERICGRAPHRUNTIME_API UGenericGraphNode : public UObject
{
	GENERATED_BODY()

public:
	UGenericGraphNode();
	virtual ~UGenericGraphNode();

	//////////////////////////////////////////////////////////////////////////
	// uproperties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GenericGraphNode")
	TSubclassOf<UObject> NodeType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GenericGraphNode")
	FString CustomNodeTitle;


	UPROPERTY(BlueprintReadOnly, Category = "GenericGraphNode")
	TArray<UGenericGraphNode*> ParentNodes;

	UPROPERTY(BlueprintReadOnly, Category = "GenericGraphNode")
	TArray<UGenericGraphNode*> ChildrenNodes;

	UPROPERTY(Transient)
	AActor* ActorOwner;

#if WITH_EDITOR
	UPROPERTY(EditDefaultsOnly, Category = "GenericGraphNode")
	FLinearColor BackgroundColor;
#endif

	//////////////////////////////////////////////////////////////////////////
	// ufunctions
	UFUNCTION(BlueprintCallable, Category = "GenericGraphNode")
	FString GetNodeTitle();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void EnterNode(AActor* OwnerActor);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ExitNode(AActor* OwnerActor);

	//////////////////////////////////////////////////////////////////////////
	UGenericGraph* GetGraph();
};
