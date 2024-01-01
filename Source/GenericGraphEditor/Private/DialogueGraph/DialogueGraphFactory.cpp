#include "DialogueGraph/DialogueGraphFactory.h"
#include "DialogueGraph/DialogueGraph.h"

#define LOCTEXT_NAMESPACE "DialogueSessionFactory"

UDialogueGraphFactory::UDialogueGraphFactory()
{
    bCreateNew = true;
    bEditAfterNew = true;
    SupportedClass = UDialogueGraph::StaticClass();
}

UObject* UDialogueGraphFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context,
    FFeedbackContext* Warn)
{
    return  NewObject<UObject>(InParent, Class, Name, Flags | RF_Transactional);
}

FText UDialogueGraphFactory::GetDisplayName() const
{
    return LOCTEXT("FactoryName", "Dialogue Graph");
}

FString UDialogueGraphFactory::GetDefaultNewAssetName() const
{
    return "DialogueGraph";
}

#undef LOCTEXT_NAMESPACE