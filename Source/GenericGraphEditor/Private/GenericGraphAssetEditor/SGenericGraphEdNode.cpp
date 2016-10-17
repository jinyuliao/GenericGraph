#include "GenericGraphEditorPrivatePCH.h"
#include "SGenericGraphEdNode.h"
#include "GenericGraphColors.h"

//////////////////////////////////////////////////////////////////////////
class SGenericGraphPin : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SGenericGraphPin) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InPin)
	{
		this->SetCursor(EMouseCursor::Default);

		bShowLabel = true;

		GraphPinObj = InPin;
		check(GraphPinObj != nullptr);

		const UEdGraphSchema* Schema = GraphPinObj->GetSchema();
		check(Schema);

		SBorder::Construct(SBorder::FArguments()
			.BorderImage(this, &SGenericGraphPin::GetPinBorder)
			.BorderBackgroundColor(this, &SGenericGraphPin::GetPinColor)
			.OnMouseButtonDown(this, &SGenericGraphPin::OnPinMouseDown)
			.Cursor(this, &SGenericGraphPin::GetPinCursor)
			.Padding(FMargin(10.0f))
		);
	}

protected:
	virtual FSlateColor GetPinColor() const override
	{
		return GenericGraphColors::Pin::Default;
	}

	virtual TSharedRef<SWidget>	GetDefaultValueWidget() override
	{
		return SNew(STextBlock);
	}

	const FSlateBrush* GetPinBorder() const
	{
		return FEditorStyle::GetBrush(TEXT("Graph.StateNode.Body"));
	}
};


//////////////////////////////////////////////////////////////////////////
void SGenericGraphEdNode::Construct(const FArguments& InArgs, UGenericGraphEdNode* InNode)
{
	GraphNode = InNode;
	UpdateGraphNode();
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SGenericGraphEdNode::UpdateGraphNode()
{
	const FMargin NodePadding = FMargin(2.0f);

	InputPins.Empty();
	OutputPins.Empty();

	// Reset variables that are going to be exposed, in case we are refreshing an already setup node.
	RightNodeBox.Reset();
	LeftNodeBox.Reset();
	OutputPinBox.Reset();

	TSharedPtr<SErrorText> ErrorText;
	TSharedPtr<STextBlock> DescriptionText;
	TSharedPtr<SNodeTitle> NodeTitle = SNew(SNodeTitle, GraphNode);

	TWeakPtr<SNodeTitle> WeakNodeTitle = NodeTitle;
	auto GetNodeTitlePlaceholderWidth = [WeakNodeTitle]() -> FOptionalSize
	{
		TSharedPtr<SNodeTitle> NodeTitlePin = WeakNodeTitle.Pin();
		const float DesiredWidth = (NodeTitlePin.IsValid()) ? NodeTitlePin->GetTitleSize().X : 0.0f;
		return FMath::Max(75.0f, DesiredWidth);
	};
	auto GetNodeTitlePlaceholderHeight = [WeakNodeTitle]() -> FOptionalSize
	{
		TSharedPtr<SNodeTitle> NodeTitlePin = WeakNodeTitle.Pin();
		const float DesiredHeight = (NodeTitlePin.IsValid()) ? NodeTitlePin->GetTitleSize().Y : 0.0f;
		return FMath::Max(22.0f, DesiredHeight);
	};

	this->ContentScale.Bind(this, &SGraphNode::GetContentScale);
	this->GetOrAddSlot(ENodeZone::Center)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Center)
		[
			SNew(SBorder)
			.BorderImage(FEditorStyle::GetBrush("Graph.StateNode.Body"))
			.Padding(0.0f)
			.BorderBackgroundColor(this, &SGenericGraphEdNode::GetBorderBackgroundColor)
			//.OnMouseButtonDown(this, &SGenericGraphEdNode::OnMouseDown)
			[
				SNew(SOverlay)

				// Pins and node details
				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SNew(SVerticalBox)

					// INPUT PIN AREA
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SBox)
						.MinDesiredHeight(NodePadding.Top)
						[
							SAssignNew(LeftNodeBox, SVerticalBox)
						]
					]

					// STATE NAME AREA
					+ SVerticalBox::Slot()
					.Padding(FMargin(NodePadding.Left, 0.0f, NodePadding.Right, 0.0f))
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SAssignNew(NodeBody, SBorder)
							.BorderImage(FEditorStyle::GetBrush("BTEditor.Graph.BTNode.Body"))
							.BorderBackgroundColor(this, &SGenericGraphEdNode::GetBackgroundColor)
							.HAlign(HAlign_Fill)
							.VAlign(VAlign_Center)
							.Visibility(EVisibility::SelfHitTestInvisible)
							[
								SNew(SOverlay)
								+ SOverlay::Slot()
								.HAlign(HAlign_Fill)
								.VAlign(VAlign_Fill)
								[
									SNew(SVerticalBox)
									+ SVerticalBox::Slot()
									.AutoHeight()
									[
										SNew(SHorizontalBox)
										+ SHorizontalBox::Slot()
										.AutoWidth()
										[
											// POPUP ERROR MESSAGE
											SAssignNew(ErrorText, SErrorText)
											.BackgroundColor(this, &SGenericGraphEdNode::GetErrorColor)
											.ToolTipText(this, &SGenericGraphEdNode::GetErrorMsgToolTip)
										]

										+ SHorizontalBox::Slot()
										.AutoWidth()
										[
											SNew(SLevelOfDetailBranchNode)
											.UseLowDetailSlot(this, &SGenericGraphEdNode::UseLowDetailNodeTitles)
											.LowDetail()
											[
												SNew(SBox)
												.WidthOverride_Lambda(GetNodeTitlePlaceholderWidth)
												.HeightOverride_Lambda(GetNodeTitlePlaceholderHeight)
											]
											.HighDetail()
											[
												SNew(SHorizontalBox)
// 												+ SHorizontalBox::Slot()
// 												.AutoWidth()
// 												.VAlign(VAlign_Center)
// 												[
// 													SNew(SImage)
// 													.Image(this, &SGenericGraphEdNode::GetNameIcon)
// 												]
												+ SHorizontalBox::Slot()
												.Padding(FMargin(4.0f, 0.0f, 4.0f, 0.0f))
												[
													SNew(SVerticalBox)
													+ SVerticalBox::Slot()
													.AutoHeight()
													[
														SAssignNew(InlineEditableText, SInlineEditableTextBlock)
														.Style(FEditorStyle::Get(), "Graph.StateNode.NodeTitleInlineEditableText")
														.Text(NodeTitle.Get(), &SNodeTitle::GetHeadTitle)
														.OnVerifyTextChanged(this, &SGenericGraphEdNode::OnVerifyNameTextChanged)
														.OnTextCommitted(this, &SGenericGraphEdNode::OnNameTextCommited)
														.IsReadOnly(this, &SGenericGraphEdNode::IsNameReadOnly)
														.IsSelected(this, &SGenericGraphEdNode::IsSelectedExclusively)
													]
													+ SVerticalBox::Slot()
													.AutoHeight()
													[
														NodeTitle.ToSharedRef()
													]
												]
											]
										]
									]
// 									+ SVerticalBox::Slot()
// 									.AutoHeight()
// 									[
// 										// DESCRIPTION MESSAGE
// 										SAssignNew(DescriptionText, STextBlock)
// 										.Visibility(this, &SGenericGraphEdNode::GetDescriptionVisibility)
// 										.Text(this, &SGenericGraphEdNode::GetDescription)
// 									]
								]
							]
						]
					]

					// OUTPUT PIN AREA
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SBox)
						.MinDesiredHeight(NodePadding.Bottom)
						[
							SAssignNew(RightNodeBox, SVerticalBox)
							+ SVerticalBox::Slot()
							.HAlign(HAlign_Fill)
							.VAlign(VAlign_Fill)
							.Padding(20.0f, 0.0f)
							.FillHeight(1.0f)
							[
								SAssignNew(OutputPinBox, SHorizontalBox)
							]
						]
					]
				]
			]
		];

	// Create comment bubble
	TSharedPtr<SCommentBubble> CommentBubble;
	const FSlateColor CommentColor = GetDefault<UGraphEditorSettings>()->DefaultCommentNodeTitleColor;

	SAssignNew(CommentBubble, SCommentBubble)
		.GraphNode(GraphNode)
		.Text(this, &SGraphNode::GetNodeComment)
		.OnTextCommitted(this, &SGraphNode::OnCommentTextCommitted)
		.ColorAndOpacity(CommentColor)
		.AllowPinning(true)
		.EnableTitleBarBubble(true)
		.EnableBubbleCtrls(true)
		.GraphLOD(this, &SGraphNode::GetCurrentLOD)
		.IsGraphNodeHovered(this, &SGraphNode::IsHovered);

	GetOrAddSlot(ENodeZone::TopCenter)
		.SlotOffset(TAttribute<FVector2D>(CommentBubble.Get(), &SCommentBubble::GetOffset))
		.SlotSize(TAttribute<FVector2D>(CommentBubble.Get(), &SCommentBubble::GetSize))
		.AllowScaling(TAttribute<bool>(CommentBubble.Get(), &SCommentBubble::IsScalingAllowed))
		.VAlign(VAlign_Top)
		[
			CommentBubble.ToSharedRef()
		];

	ErrorReporting = ErrorText;
	ErrorReporting->SetError(ErrorMsg);
	CreatePinWidgets();
}

void SGenericGraphEdNode::CreatePinWidgets()
{
	UGenericGraphEdNode* StateNode = CastChecked<UGenericGraphEdNode>(GraphNode);

	for (int32 PinIdx = 0; PinIdx < StateNode->Pins.Num(); PinIdx++)
	{
		UEdGraphPin* MyPin = StateNode->Pins[PinIdx];
		if (!MyPin->bHidden)
		{
			TSharedPtr<SGraphPin> NewPin = SNew(SGenericGraphPin, MyPin);

			AddPin(NewPin.ToSharedRef());
		}
	}
}

void SGenericGraphEdNode::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	PinToAdd->SetOwner(SharedThis(this));

	const UEdGraphPin* PinObj = PinToAdd->GetPinObj();
	const bool bAdvancedParameter = PinObj && PinObj->bAdvancedView;
	if (bAdvancedParameter)
	{
		PinToAdd->SetVisibility( TAttribute<EVisibility>(PinToAdd, &SGraphPin::IsPinVisibleAsAdvanced) );
	}

	if (PinToAdd->GetDirection() == EEdGraphPinDirection::EGPD_Input)
	{
		LeftNodeBox->AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.FillHeight(1.0f)
			.Padding(20.0f,0.0f)
			[
				PinToAdd
			];
		InputPins.Add(PinToAdd);
	}
	else // Direction == EEdGraphPinDirection::EGPD_Output
	{
		OutputPinBox->AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.FillWidth(1.0f)
			[
				PinToAdd
			];
		OutputPins.Add(PinToAdd);
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

FSlateColor SGenericGraphEdNode::GetBorderBackgroundColor() const
{
	return GenericGraphColors::NodeBorder::HighlightAbortRange0;
}

FSlateColor SGenericGraphEdNode::GetBackgroundColor() const
{
	return GenericGraphColors::NodeBody::Default;
}

EVisibility SGenericGraphEdNode::GetDragOverMarkerVisibility() const
{
	return EVisibility::Visible;
}

FText SGenericGraphEdNode::GetDescription() const
{
	UGenericGraphEdNode* MyNode = CastChecked<UGenericGraphEdNode>(GraphNode);
	return MyNode ? MyNode->GetDescription() : FText::GetEmpty();
}

EVisibility SGenericGraphEdNode::GetDescriptionVisibility() const
{
	//return (GetCurrentLOD() > EGraphRenderingLOD::LowDetail) ? EVisibility::Visible : EVisibility::Collapsed;
	return EVisibility::Hidden;
}

const FSlateBrush* SGenericGraphEdNode::GetNameIcon() const
{
	return FEditorStyle::GetBrush(TEXT("BTEditor.Graph.BTNode.Icon"));
}
