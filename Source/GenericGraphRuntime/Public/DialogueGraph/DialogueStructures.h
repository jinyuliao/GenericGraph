﻿#pragma once

#include "CoreMinimal.h"
#include "DialogueStructures.generated.h"

USTRUCT(BlueprintType)
struct FDialogueVisualData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual Data", meta = (MultiLine="true"))
    FText DialogueText;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual Data")
    UTexture2D* Icon;
};

USTRUCT(BlueprintType)
struct FDialogueConditionData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Conditions")
    TArray<FString> ConditionTokens;
};

USTRUCT(BlueprintType)
struct FDialogueResultData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Results")
    TArray<FString> ResultTokens;
};