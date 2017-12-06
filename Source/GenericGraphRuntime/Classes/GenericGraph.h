#pragma once

#include "CoreMinimal.h"
#include "GenericGraphNode.h"
#include "GenericGraphEdge.h"
#include "GenericGraph.generated.h"

UCLASS(Blueprintable)
class GENERICGRAPHRUNTIME_API UGenericGraph : public UObject
{
	GENERATED_BODY()

public:
	UGenericGraph();
	virtual ~UGenericGraph();

	//////////////////////////////////////////////////////////////////////////
	// uproperties
	UPROPERTY(EditAnywhere, Category = "GenericGraph")
	FString Name;

	UPROPERTY(EditAnywhere, Category = "GenericGraph")
	TSubclassOf<UGenericGraphNode> NodeType;

	UPROPERTY(EditAnywhere, Category = "GenericGraph")
	TSubclassOf<UGenericGraphEdge> EdgeType;

	UPROPERTY(BlueprintReadOnly, Category = "GenericGraph")
	TArray<UGenericGraphNode*> RootNodes;

	UPROPERTY(BlueprintReadOnly, Category = "GenericGraph")
	TArray<UGenericGraphNode*> AllNodes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GenericGraph")
	bool bEdgeEnabled;

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	class UEdGraph* EdGraph;
#endif

	//////////////////////////////////////////////////////////////////////////
	// ufunctions
	UFUNCTION(BlueprintCallable, Category = "GenericGraph")
	void Print(bool ToConsole = true, bool ToScreen = true);

	UFUNCTION(BlueprintCallable, Category = "GenericGraph")
	int GetLevelNum();

	UFUNCTION(BlueprintCallable, Category = "GenericGraph")
	void GetNodesByLevel(int Level, TArray<UGenericGraphNode*>& Nodes);

	void ClearGraph();
};
