// Copyright Daniel Sellers 2024.


#include "PlannerStructs.h"

PlannerStructs::PlannerStructs()
{
}

PlannerStructs::~PlannerStructs()
{
}

// Stand-in implementation: will eventually print all preconditions and effects
FString FAction::ToString() const
{
    TStringBuilderWithBuffer<TCHAR, 256> StringBuilder;
    StringBuilder.Append(TEXT("\n"));
    StringBuilder.Append(TEXT("////////////////"));
    StringBuilder.Append(TEXT("\n"));
    StringBuilder.Append(*ActionName.ToString());
    StringBuilder.Append(TEXT("\n"));
    StringBuilder.Append(TEXT("Preconditions: "));
    StringBuilder.Append(TEXT("\n"));
    for (auto Precond : Preconditions)
    {
        StringBuilder.Append(*Precond.Key.ToString());
        StringBuilder.Append(TEXT(" : "));
        StringBuilder.Append(Precond.Value ? TEXT("true") : TEXT("false"));
        StringBuilder.Append("\n");
    }
    StringBuilder.Append(TEXT("Effects: "));
    StringBuilder.Append(TEXT("\n"));

    for (auto Effect : Effects)
    {
        StringBuilder.Append(*Effect.Key.ToString());
        StringBuilder.Append(TEXT(" : "));
        StringBuilder.Append(Effect.Value ? TEXT("true") : TEXT("false"));
        StringBuilder.Append("\n");
    }
    FString Result = StringBuilder.ToString();
    
    return Result;
}

