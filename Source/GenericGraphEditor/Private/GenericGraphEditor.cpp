// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "GenericGraphEditorPrivatePCH.h"
#include "GenericGraphAssetTypeActions.h"

DEFINE_LOG_CATEGORY(GenericGraphEditor)

#define LOCTEXT_NAMESPACE "GenericGraphEditor"

class FGenericGraphEditor : public IGenericGraphEditor
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action);

private:
	TArray< TSharedPtr<IAssetTypeActions> > CreatedAssetTypeActions;

	EAssetTypeCategories::Type GenericGraphAssetCategoryBit;
};

IMPLEMENT_MODULE( FGenericGraphEditor, UGenericGraphEditor )

void FGenericGraphEditor::StartupModule()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	GenericGraphAssetCategoryBit = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("GenericGraph")), LOCTEXT("GenericGraphAssetCategory", "GenericGraph"));

	RegisterAssetTypeAction(AssetTools, MakeShareable(new FGenericGraphAssetTypeActions(GenericGraphAssetCategoryBit)));
}


void FGenericGraphEditor::ShutdownModule()
{
	// Unregister all the asset types that we registered
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
		for (int32 Index = 0; Index < CreatedAssetTypeActions.Num(); ++Index)
		{
			AssetTools.UnregisterAssetTypeActions(CreatedAssetTypeActions[Index].ToSharedRef());
		}
	}
}

void FGenericGraphEditor::RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action)
{
	AssetTools.RegisterAssetTypeActions(Action);
	CreatedAssetTypeActions.Add(Action);
}

#undef LOCTEXT_NAMESPACE

