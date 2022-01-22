#ifndef _HubProperty_h
#define _HubProperty_h

#include "BaseProperty.h"

class HubProperty : public BaseProperty
{
public:
	HubConnectionStatus Status;
	String ProjectId;
	String Mac;

	String Serialize();
	void FillJson(JsonObject* obj); 
	bool ChangeProperty(const char* propertyName, const char* jsonData, PropertyChangedBy propertyChangedBy = PropertyChangedBy_MyCode);

	HubProperty() : BaseProperty(0, "Hub")
	{
		PropertyType = WellKnowObjectIds_Hub;
		PropertyId = 0;
	}
};

#endif