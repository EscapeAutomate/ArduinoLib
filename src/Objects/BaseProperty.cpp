#include "BaseProperty.h"
#include "EscapeAutomate.hpp"

void BaseProperty::FillBaseJson(JsonObject* doc)
{
    (*doc)["name"] = Name;
    (*doc)["id"] = PropertyId;
    (*doc)["propertyType"] = PropertyType;
}

bool BaseProperty::SendPuzzlePropertyChanged(uint16_t propertyId, String propertyName, String value)
{
    return EscapeAutomate.SendPuzzlePropertyChanged(ParentPuzzleId, propertyId, propertyName, value);
}
