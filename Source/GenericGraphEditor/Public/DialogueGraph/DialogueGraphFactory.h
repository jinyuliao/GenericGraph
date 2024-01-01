#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "DialogueGraphFactory.generated.h"

UCLASS()
class UDialogueGraphFactory : public UFactory
{
    GENERATED_BODY()
public:
    UDialogueGraphFactory();

    virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
    virtual FText GetDisplayName() const override;
    virtual FString GetDefaultNewAssetName() const override;
};
