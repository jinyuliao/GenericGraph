// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "GenericGraphEditorPrivatePCH.h"
#include "GenericGraphAssetTypeActions.h"
#include "GenericGraphAssetEditor/GenericGraphAssetEditor.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions"

FGenericGraphAssetTypeActions::FGenericGraphAssetTypeActions(EAssetTypeCategories::Type InAssetCategory)
	: MyAssetCategory(InAssetCategory)
{
}

FText FGenericGraphAssetTypeActions::GetName() const
{
	return LOCTEXT("FGenericGraphAssetTypeActionsName", "Generic Graph");
}

FColor FGenericGraphAssetTypeActions::GetTypeColor() const
{
	return FColor(129, 196, 115);
}

UClass* FGenericGraphAssetTypeActions::GetSupportedClass() const
{
	return UGenericGraph::StaticClass();
}

void FGenericGraphAssetTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		if (UGenericGraph* Graph = Cast<UGenericGraph>(*ObjIt))
		{
			TSharedRef<FGenericGraphAssetEditor> NewGraphEditor(new FGenericGraphAssetEditor());
			NewGraphEditor->InitGenericGraphAssetEditor(Mode, EditWithinLevelEditor, Graph);
		}
	}
}

uint32 FGenericGraphAssetTypeActions::GetCategories()
{
	return EAssetTypeCategories::Animation | MyAssetCategory;
}

//////////////////////////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE