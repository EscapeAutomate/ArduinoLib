#include "NumberProperty.h"
#include "EscapeAutomate.hpp"
#include "BoolProperty.h"

extern EscapeAutomateClass EscapeAutomate;

String NumberProperty::Serialize()
{
    DynamicJsonDocument doc(500);

    JsonObject object = doc.to<JsonObject>();
    FillJson(&object);

    String str;

    serializeJson(doc, str);

    return str;
}

void NumberProperty::FillJson(JsonObject* doc)
{
    FillBaseJson(doc);
    (*doc)["value"] = Value;
}

bool NumberProperty::ChangeProperty(const char* propertyName, const char* jsonData)
{
    if (strcmp(propertyName, "Value") == 0)
    {
        StaticJsonDocument<64> doc;

        DeserializationError error = deserializeJson(doc, jsonData);

        if (error) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
            return false;
        }

        this->Value = doc.as<float_t>();
        return EscapeAutomate.SendPuzzlePropertyChanged(ParentPuzzleId, PropertyId, propertyName, jsonData);
    }
    else
    {
        ESC_LOGERROR1("Undefined Property: ", propertyName);
    }

    return false;
}

bool NumberProperty::ChangeProperty(float_t value)
{
    StaticJsonDocument<20> doc;
    this->Value = value;

    doc.set(Value);

    String json;

    serializeJson(doc, json);

    return EscapeAutomate.SendPuzzlePropertyChanged(ParentPuzzleId, PropertyId, "value", json);
}
