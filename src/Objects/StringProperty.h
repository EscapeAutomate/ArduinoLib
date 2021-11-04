#ifndef _StringProperty_h
#define _StringProperty_h

#include "BaseProperty.h"

class StringProperty : public BaseProperty
{
public:
	String Value;

	String Serialize();
	void FillJson(JsonObject* doc);
	bool ChangeProperty(const char* propertyName, const char* jsonData);
	bool ChangeProperty(String value);

	StringProperty(uint16_t propertyId, String name, String value = "") : BaseProperty(propertyId, name)
	{
		PropertyType = WellKnowObjectIds_String;
		Value = value;
	}
};

#endif