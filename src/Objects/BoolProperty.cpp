#include "BoolProperty.h"
#include "EscapeAutomate.hpp"

extern EscapeAutomateClass EscapeAutomate;

String BoolProperty::Serialize()
{
	DynamicJsonDocument doc(500);

	JsonObject object = doc.to<JsonObject>();
	FillJson(&object);

	String str;

	serializeJson(doc, str);

	return str;
}

void BoolProperty::FillJson(JsonObject* doc)
{
	FillBaseJson(doc);
	(*doc)["value"] = Value;
}

bool BoolProperty::ChangeProperty(const char* propertyName, const char* jsonData, PropertyChangedBy propertyChangedBy)
{
	if (strcmp(propertyName, "Value") == 0)
	{
		StaticJsonDocument<100> doc;

		DeserializationError error = deserializeJson(doc, jsonData);

		if (error) {
			Serial.print(F("deserializeJson() failed: "));
			Serial.println(error.f_str());
			return false;
		}

		this->Value = doc.as<bool>();

		return EscapeAutomate.SendPuzzlePropertyChanged(ParentPuzzleId, PropertyId, propertyName, jsonData, propertyChangedBy);

	}
	else
	{
		ESC_LOGERROR1("Undefined Property: ", propertyName);

	}

	return false;
}

bool BoolProperty::ChangeProperty(bool value, PropertyChangedBy propertyChangedBy)
{
	this->Value = value;

	StaticJsonDocument<20> doc;

	doc.set(Value);

	String json;

	serializeJson(doc, json);

	return EscapeAutomate.SendPuzzlePropertyChanged(ParentPuzzleId, PropertyId, "value", json, propertyChangedBy);
}
