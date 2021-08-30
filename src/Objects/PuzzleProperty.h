#ifndef _PuzzleProperty_h
#define _PuzzleProperty_h

#include "BaseProperty.h"

class PuzzleProperty : public BaseProperty
{
public:
	PuzzleStatus Status;
	String Notification;
	Difficulty DifficultyValue;
	uint16_t PuzzleId;

	String Serialize();
	void FillJson(JsonObject* obj);
	bool ChangeProperty(uint16_t puzzleId, const char* propertyName, const char* jsonData);

	PuzzleProperty(String name) : BaseProperty(0, name)
	{
		PropertyType = WellKnowObjectIds_Puzzle;
		Status = PuzzleStatus_Disconnected;

		PropertyId = 0;
	}
};

#endif