#ifndef _StringProperty_h
#define _StringProperty_h

#include "BaseProperty.h"

class StringProperty : public BaseProperty
{
public:
	String Value;

	String Serialize();
	void FillJson(JsonObject* doc);
	bool ChangeProperty(uint16_t puzzleId, const char* propertyName, const char* jsonData);
	bool ChangeProperty(uint16_t puzzleId, String value);

	StringProperty(uint16_t propertyId, String name) : BaseProperty(propertyId, name)
	{
		PropertyType = WellKnowObjectIds_String;
	}
};

#endif