#include "PuzzleProperty.h"
#include "EscapeAutomate.hpp"
#include "EscLogs.h"

String PuzzleProperty::Serialize()
{
    DynamicJsonDocument doc(2048);
    String json;

    JsonObject object = doc.to<JsonObject>();
    FillJson(&object);

    serializeJson(doc, json);

    return json;
}

void PuzzleProperty::FillJson(JsonObject* obj)
{
    FillBaseJson(obj);
    (*obj)["status"] = Status;
    (*obj)["notification"] = Notification;
    (*obj)["difficulty"] = DifficultyValue;
    (*obj)["puzzleId"] = PuzzleId;
}

bool PuzzleProperty::ChangeProperty(const char* propertyName, const char* jsonData)
{
    if (strcmp(propertyName, "Status") == 0)
    {
        StaticJsonDocument<20> doc;

        DeserializationError error = deserializeJson(doc, jsonData);

        if (error) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
            return false;
        }

        PuzzleStatus status = (PuzzleStatus)doc.as<uint16_t>();

        EscapeAutomate.ManagePuzzleStatusChange(ParentPuzzleId, status);
        return EscapeAutomate.SendPuzzlePropertyChanged(ParentPuzzleId, PropertyId, propertyName, jsonData);
    }
    else if (strcmp(propertyName,"Difficulty") == 0)
    {
        StaticJsonDocument<256> doc;

        DeserializationError error = deserializeJson(doc, jsonData);

        if (error) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
            return false;
        }

        int difficulty = doc.as<uint16_t>();

        this->DifficultyValue = (Difficulty)difficulty;

        return EscapeAutomate.SendPuzzlePropertyChanged(ParentPuzzleId, PropertyId, propertyName, jsonData);
    }
    else
    {
        ESC_LOGERROR1("Undefined Property: ", propertyName);
    }

    return false;
}
