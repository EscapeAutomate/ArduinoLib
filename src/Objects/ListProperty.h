#ifndef _ListProperty_h
#define _ListProperty_h

#include "BaseProperty.h"
#include <map>
#include <vector>


class ListProperty : public BaseProperty
{
public:
	bool AllowMultiSelect;
	bool AllowZero;
	std::map<uint16_t, String> Values;
	std::vector<uint16_t> SelectedValues;

	ListProperty(uint16_t propertyId, String name, bool allowZero, bool allowMulti, const String* values, int sizeValues, String* selectedValues, int sizeSelected);

	String Serialize();
	void FillJson(JsonObject* doc);
	bool ChangeProperty(const char* propertyName, const char* jsonData);

	void AddValue(String string);

	bool SelectValue(String string);

	bool UnselectValue(uint16_t value);

	bool ClearSelected();

	bool ClearSelected(String value);
};

#endif