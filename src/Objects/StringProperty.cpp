#include "StringProperty.h"
#include "EscapeAutomate.hpp"

extern EscapeAutomateClass EscapeAutomate;

String StringProperty::Serialize()
{
    DynamicJsonDocument doc(1024);

    JsonObject object = doc.to<JsonObject>();
    FillJson(&object);

    String str;

    serializeJson(doc, str);

    return str;
}

void StringProperty::FillJson(JsonObject* doc)
{
    FillBaseJson(doc);
    (*doc)["value"] = Value;
}

bool StringProperty::ChangeProperty(const char* propertyName, const char* jsonData, PropertyChangedBy propertyChangedBy)
{
    if (strcmp(propertyName, "Value") == 0)
    {
        StaticJsonDocument<512> doc;

        DeserializationError error = deserializeJson(doc, jsonData);

        if (error) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
            return false;
        }

        this->Value = doc.as<String>();

        return EscapeAutomate.SendPuzzlePropertyChanged(ParentPuzzleId, PropertyId, propertyName, jsonData, propertyChangedBy);
    }
    else 
    {
        ESC_LOGERROR1("Undefined Property: ", propertyName);
    }

    return false;
}

bool StringProperty::ChangeProperty(String value, PropertyChangedBy propertyChangedBy)
{
    StaticJsonDocument<20> doc;
    this->Value = value;

    doc.set(Value);

    String json;

    serializeJson(doc, json);

    return EscapeAutomate.SendPuzzlePropertyChanged(ParentPuzzleId, PropertyId, "value", json, propertyChangedBy);
}
