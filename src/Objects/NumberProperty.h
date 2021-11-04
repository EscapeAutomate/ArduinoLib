#ifndef _NumberProperty_h
#define _NumberProperty_h

#include "BaseProperty.h"

class NumberProperty : public BaseProperty
{
public:
	float_t Value;

	String Serialize();
	void FillJson(JsonObject* doc);
	bool ChangeProperty(const char* propertyName, const char* jsonData);
	bool ChangeProperty(float_t value);

	NumberProperty(uint16_t propertyId, String name, float_t value = 0) : BaseProperty(propertyId, name)
	{
		PropertyType = WellKnowObjectIds_Int;
		Value = value;
	}
};

#endif