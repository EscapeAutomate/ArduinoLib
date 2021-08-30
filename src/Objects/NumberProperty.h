#ifndef _NumberProperty_h
#define _NumberProperty_h

#include "BaseProperty.h"

class NumberProperty : public BaseProperty
{
public:
	float_t Value;

	String Serialize();
	void FillJson(JsonObject* doc);
	bool ChangeProperty(uint16_t puzzleId, const char* propertyName, const char* jsonData);
	bool ChangeProperty(uint16_t puzzleId, float_t value);

	NumberProperty(uint16_t propertyId, String name) : BaseProperty(propertyId, name)
	{
		PropertyType = WellKnowObjectIds_Int;
		Value = 0;
	}
};

#endif