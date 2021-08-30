#include "BaseProperty.h"
#include "EscapeAutomate2.hpp"

void BaseProperty::FillBaseJson(JsonObject* doc)
{
    (*doc)["name"] = Name;
    (*doc)["id"] = PropertyId;
    (*doc)["propertyType"] = PropertyType;
}

bool BaseProperty::SendPuzzlePropertyChanged(uint16_t puzzleId, uint16_t propertyId, String propertyName, String value)
{
    return EscapeAutomate.SendPuzzlePropertyChanged(puzzleId, propertyId, propertyName, value);
}
