#ifndef _BaseProperty_h
#define _BaseProperty_h

#include <Enums.h>
#include <ArduinoJson.h>

class BaseProperty
{
public:
	virtual String Serialize() = 0;
	virtual bool ChangeProperty(uint16_t puzzleId, const char* propertName, const char* jsonData) = 0;

	void FillBaseJson(JsonObject* doc);
	bool SendPuzzlePropertyChanged(uint16_t puzzleId, uint16_t propertyId, String propertyName, String value);

	BaseProperty(uint16_t propertyId, String name)
	{
		Name = name;
		PropertyId = propertyId;
	}

	uint16_t PropertyId;
	String Name;
	uint16_t PropertyType;
	uint16_t ParentPuzzleId;
};

#endif