// Copyright Daniel Sellers 2024.


#include "PlannerStructs.h"

PlannerStructs::PlannerStructs()
{
}

PlannerStructs::~PlannerStructs()
{
}

FString FAction::ToString() const
{
    return *ActionName.ToString();
}
