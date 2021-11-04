#include "EscapeAutomate.hpp"
#include "defines.h"
#include "ListProperty.h"
#include <map>

extern EscapeAutomateClass EscapeAutomate;

ListProperty::ListProperty(uint16_t propertyId, String name, bool allowZero, bool allowMulti, const String* values, int sizeValues, String* selectedValues, int sizeSelected) : BaseProperty(propertyId, name)
{
	PropertyType = WellKnowObjectIds_List;

	AllowMultiSelect = allowMulti;
	AllowZero = allowZero;

	for (int i = 0; i < sizeValues; i++) {
		Values[i] = values[i];
	}

	for (int i = 0; i < sizeSelected; i++) {
		SelectValue(selectedValues[i]);
	}

	if (!allowZero && (sizeSelected == 0))
	{
		ESC_LOGERROR1(name, ": You must have at least one selected value because allowZero property is set to false");
	}
	else if (!allowMulti && (sizeSelected >= 2))
	{
		ESC_LOGERROR1(name, ": You can not have more than 1 selected values because allowMulti property is set to false");
	}
}

String ListProperty::Serialize()
{
    DynamicJsonDocument doc(1024);

    JsonObject object = doc.to<JsonObject>();
    FillJson(&object);

    String str;

    serializeJson(doc, str);

    return str;
}

void ListProperty::FillJson(JsonObject* doc)
{
    FillBaseJson(doc);
   // (*doc)["values"] = Values;
	
    (*doc)["allowMultiSelect"] = AllowMultiSelect;
	(*doc)["allowZero"] = AllowZero;

    JsonObject values = (*doc).createNestedObject("values");
	for (std::pair <uint16_t, String> value : Values)
	{
		values[(String)value.first] = value.second;
	}

    JsonArray selectedValues = (*doc).createNestedArray("selectedValues");
	for (uint16_t value : SelectedValues)
	{
		selectedValues.add(value);
	}
}

bool ListProperty::ChangeProperty(const char* propertyName, const char* jsonData)
{
	if (strcmp(propertyName, "SelectedValues") == 0)
	{
		DynamicJsonDocument doc(1024);
		
		DeserializationError error = deserializeJson(doc, jsonData);

		if (error) {
			Serial.print(F("deserializeJson() failed: "));
			Serial.println(error.f_str());
			return false;
		}
		
		SelectedValues.clear();

		for (int i; i<doc.size(); i++ )
		{
			this->SelectedValues.insert(SelectedValues.end(), doc[i].as<uint16_t>());
		}

		return EscapeAutomate.SendPuzzlePropertyChanged(ParentPuzzleId, PropertyId, propertyName, jsonData);
	}
	else
	{
		ESC_LOGERROR1("Undefined Property: ", propertyName);
	}
	return false;
	
}

void ListProperty::AddValue(String string)
{
	Values.insert(std::pair<uint16_t, String>(Values.size(), string));
}

bool ListProperty::SelectValue(String string)
{
	if (AllowMultiSelect || SelectedValues.size() == 0)
	{
		for (auto var : Values)
		{
			if (var.second == string)
			{
				SelectedValues.insert(SelectedValues.end(), var.first);
				return true;
			}
		}
	}
	return false;
}

bool ListProperty::UnselectValue(uint16_t value)
{
	if (AllowZero || (!AllowZero && SelectedValues.size() > 1))
	{
		for (uint16_t i = 0; i < SelectedValues.size(); ++i)
		{
			if (SelectedValues[i] == value)
			{
				SelectedValues.erase(SelectedValues.begin() + i);
				return true;
			}
		}
	}

	return false;
}

bool ListProperty::ClearSelected()
{
	if (AllowZero) {
		SelectedValues.clear();
		return true;
	}

	return false;
}

bool ListProperty::ClearSelected(String string)
{
	for (uint16_t i = 0; i < SelectedValues.size(); ++i)
	{
		for (auto var : Values)
		{
			if (var.second == string)
			{
				SelectedValues.clear();
				SelectedValues.insert(SelectedValues.end(), var.first);
				return true;
			}
		}
	}

	return false;
}
