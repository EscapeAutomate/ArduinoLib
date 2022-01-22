#include "BaseProperty.h"
#include "EscapeAutomate.hpp"

void BaseProperty::FillBaseJson(JsonObject* doc)
{
    (*doc)["name"] = Name;
    (*doc)["id"] = PropertyId;
    (*doc)["propertyType"] = PropertyType;
}
