#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateColor.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SWidget.h"
#include "SNodePanel.h"
#include "SGraphNode.h"

class SToolTip;

class SEdNode_GenericGraphEdge : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SEdNode_GenericGraphEdge){}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdNode_GenericGraphEdge* InNode);

	// SNodePanel::SNode interface
	virtual void MoveTo(const FVector2D& NewPosition, FNodeSet& NodeFilter) override;
	virtual bool RequiresSecondPassLayout() const override;
	virtual void PerformSecondPassLayout(const TMap< UObject*, TSharedRef<SNode> >& NodeToWidgetLookup) const override;
	// End of SNodePanel::SNode interface

	virtual void UpdateGraphNode() override;

	// Calculate position for multiple nodes to be placed between a start and end point, by providing this nodes index and max expected nodes 
	void PositionBetweenTwoNodesWithOffset(const FGeometry& StartGeom, const FGeometry& EndGeom, int32 NodeIndex, int32 MaxNodes) const;

protected:
	FSlateColor GetTransitionColor() const;

private:
	TSharedPtr<STextEntryPopup> TextEntryWidget;
};
