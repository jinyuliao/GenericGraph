#pragma once

#include "GenericGraphAssetGraphSchema.generated.h"

/** Action to add a node to the graph */
USTRUCT()
struct FGenericGraphAssetSchemaAction_NewNode : public FEdGraphSchemaAction
{
	GENERATED_USTRUCT_BODY();

	/** Class of node we want to create */
	UPROPERTY()
	class UClass* NodeData;

	FGenericGraphAssetSchemaAction_NewNode()
		: FEdGraphSchemaAction()
		, NodeData(NULL)
	{}

	FGenericGraphAssetSchemaAction_NewNode(const FText& InNodeCategory, const FText& InMenuDesc, const FString& InToolTip, const int32 InGrouping)
		: FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping) 
		, NodeData(NULL)
	{}

	//~ Begin FEdGraphSchemaAction Interface
	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;
	//~ End FEdGraphSchemaAction Interface

private:
	/** Connects new node to output of selected nodes */
	void ConnectToSelectedNodes(USoundNode* NewNodeclass, UEdGraph* ParentGraph) const;
};
// 
// /** Action to add nodes to the graph based on selected objects*/
// USTRUCT()
// struct UNREALED_API FGenericGraphAssetSchemaAction_NewFromSelected : public FSoundCueGraphSchemaAction_NewNode
// {
// 	GENERATED_USTRUCT_BODY();
// 
// 	FSoundCueGraphSchemaAction_NewFromSelected() 
// 		: FSoundCueGraphSchemaAction_NewNode()
// 	{}
// 
// 	FSoundCueGraphSchemaAction_NewFromSelected(const FText& InNodeCategory, const FText& InMenuDesc, const FString& InToolTip, const int32 InGrouping)
// 		: FSoundCueGraphSchemaAction_NewNode(InNodeCategory, InMenuDesc, InToolTip, InGrouping) 
// 	{}
// 
// 	//~ Begin FEdGraphSchemaAction Interface
// 	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;
// 	//~ End FEdGraphSchemaAction Interface
// };
// 
// /** Action to create new comment */
// USTRUCT()
// struct UNREALED_API FGenericGraphAssetSchemaAction_NewComment : public FEdGraphSchemaAction
// {
// 	GENERATED_USTRUCT_BODY();
// 
// 	FSoundCueGraphSchemaAction_NewComment() 
// 		: FEdGraphSchemaAction()
// 	{}
// 
// 	FSoundCueGraphSchemaAction_NewComment(const FText& InNodeCategory, const FText& InMenuDesc, const FString& InToolTip, const int32 InGrouping)
// 		: FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping)
// 	{}
// 
// 	//~ Begin FEdGraphSchemaAction Interface
// 	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;
// 	//~ End FEdGraphSchemaAction Interface
// };
// 
// /** Action to paste clipboard contents into the graph */
// USTRUCT()
// struct UNREALED_API FGenericGraphAssetSchemaAction_Paste : public FEdGraphSchemaAction
// {
// 	GENERATED_USTRUCT_BODY();
// 
// 	FSoundCueGraphSchemaAction_Paste() 
// 		: FEdGraphSchemaAction()
// 	{}
// 
// 	FSoundCueGraphSchemaAction_Paste(const FText& InNodeCategory, const FText& InMenuDesc, const FString& InToolTip, const int32 InGrouping)
// 		: FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping)
// 	{}
// 
// 	//~ Begin FEdGraphSchemaAction Interface
// 	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;
// 	//~ End FEdGraphSchemaAction Interface
// };

UCLASS(MinimalAPI)
class UGenericGraphAssetGraphSchema : public UEdGraphSchema
{
	GENERATED_BODY()

	/** Check whether connecting these pins would cause a loop */
	bool ConnectionCausesLoop(const UEdGraphPin* InputPin, const UEdGraphPin* OutputPin) const;

	/** Attempts to connect the output of multiple nodes to the inputs of a single one */
	void TryConnectNodes(const TArray<USoundNode*>& OutputNodes, USoundNode* InputNode) const;

	//~ Begin EdGraphSchema Interface
 	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
// 	virtual void GetContextMenuActions(const UEdGraph* CurrentGraph, const UEdGraphNode* InGraphNode, const UEdGraphPin* InGraphPin, class FMenuBuilder* MenuBuilder, bool bIsDebugging) const override;
// 	virtual void CreateDefaultNodesForGraph(UEdGraph& Graph) const override;
// 	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;
// 	virtual bool TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const override;
// 	virtual bool ShouldHidePinDefaultValue(UEdGraphPin* Pin) const override;
// 	virtual FLinearColor GetPinTypeColor(const FEdGraphPinType& PinType) const override;
// 	virtual void BreakNodeLinks(UEdGraphNode& TargetNode) const override;
// 	virtual void BreakPinLinks(UEdGraphPin& TargetPin, bool bSendsNodeNotifcation) const override;
// 	virtual void DroppedAssetsOnGraph(const TArray<class FAssetData>& Assets, const FVector2D& GraphPosition, UEdGraph* Graph) const override;
// 	virtual void DroppedAssetsOnNode(const TArray<FAssetData>& Assets, const FVector2D& GraphPosition, UEdGraphNode* Node) const override;
// 	virtual int32 GetNodeSelectionCount(const UEdGraph* Graph) const override;
// 	virtual TSharedPtr<FEdGraphSchemaAction> GetCreateCommentAction() const override;
	//~ End EdGraphSchema Interface

private:
	/** Adds actions for creating every type of SoundNode */
	void GetAllSoundNodeActions(FGraphActionMenuBuilder& ActionMenuBuilder, bool bShowSelectedActions) const;
	/** Adds action for creating a comment */
	void GetCommentAction(FGraphActionMenuBuilder& ActionMenuBuilder, const UEdGraph* CurrentGraph = NULL) const;

private:
	/** Generates a list of all available SoundNode classes */
	static void InitSoundNodeClasses();

	/** A list of all available Node classes */
	static TArray<UClass*> NodeClasses;
	/** Whether the list of Node classes has been populated */
	static bool bNodeClassesInitialized;
};

