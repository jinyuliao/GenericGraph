#pragma once

#include "CoreMinimal.h"
#include "AutoLayoutStrategy.h"
#include "ForceDirectedLayoutStrategy.generated.h"

UCLASS()
class GENERICGRAPHEDITOR_API UForceDirectedLayoutStrategy : public UAutoLayoutStrategy
{
	GENERATED_BODY()
public:
	UForceDirectedLayoutStrategy();
	virtual ~UForceDirectedLayoutStrategy();

	virtual void Layout(UEdGraph* EdGraph) override;

protected:
	virtual FBox2D LayoutOneTree(UGenericGraphNode* RootNode, const FBox2D& PreTreeBound);

protected:
	bool bRandomInit;
	float InitTemperature;
	float CoolDownRate;
};
