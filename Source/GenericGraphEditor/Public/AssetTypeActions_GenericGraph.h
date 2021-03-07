#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"

class GENERICGRAPHEDITOR_API FAssetTypeActions_GenericGraph : public FAssetTypeActions_Base
{
public:
	FAssetTypeActions_GenericGraph(EAssetTypeCategories::Type InAssetCategory);

	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;
	virtual uint32 GetCategories() override;

private:
	EAssetTypeCategories::Type MyAssetCategory;
};