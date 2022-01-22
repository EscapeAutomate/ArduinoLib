#include "HubProperty.h"
#include "EscapeAutomate.hpp"

String HubProperty::Serialize()
{
    DynamicJsonDocument doc(1024);
    String json;

    JsonObject object = doc.to<JsonObject>();
    FillJson(&object);

    serializeJson(doc, json);

    return json;
}

void HubProperty::FillJson(JsonObject* obj)
{
    FillBaseJson(obj);
    (*obj)["status"] = Status;
    (*obj)["projectId"] = ProjectId;
    (*obj)["mac"] = Mac;
}

bool HubProperty::ChangeProperty(const char* propertyName, const char* jsonData, PropertyChangedBy propertyChangedBy)
{
    return false;
}
