#ifndef _BoolProperty_h
#define _BoolProperty_h

#include "BaseProperty.h"

class BoolProperty : public BaseProperty
{
public:
	bool Value;

	String Serialize();
	void FillJson(JsonObject* doc);
	bool ChangeProperty(uint16_t puzzleId, const char* propertyName, const char* jsonData);
	bool ChangeProperty(uint16_t puzzleId, bool value);

	BoolProperty(uint16_t propertyId, String name) : BaseProperty(propertyId, name)
	{
		PropertyType = WellKnowObjectIds_Bool;
		Value = true;
	}
};

#endif