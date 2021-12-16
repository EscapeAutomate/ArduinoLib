#ifndef _PUZZLE_h
#define _PUZZLE_h

#include <map>
#include <ArduinoJson.h>
#include "Objects/BoolProperty.h"
#include "Objects/NumberProperty.h"
#include "Objects/StringProperty.h"
#include "Objects/ListProperty.h"
#include "Objects/PuzzleProperty.h"

class Puzzle
{
public:
	std::map<uint16_t, BaseProperty*> Properties;
	PuzzleProperty* PuzzleObject;

	Puzzle(uint16_t puzzleId, String name, Difficulty difficulty = Difficulty_Normal)
	{
		PuzzleObject = new PuzzleProperty(name);
		PuzzleObject->DifficultyValue = difficulty;
		PuzzleObject->PuzzleId = puzzleId;
		PuzzleObject->Status = PuzzleStatus_Disconnected;
	}

	void RegisterProperty(BaseProperty* prop);
	void UpdatePuzzleStatus(PuzzleStatus status);
	static void InnerLoop(void* parameter);

	virtual void Setup() = 0;
	virtual void Start() = 0;
	virtual void Stop() = 0;
	virtual void Reset() = 0;
	virtual void Loop() = 0;
	virtual void Notification(uint16_t senderId, const char* jsonValue) = 0;
	virtual void Completed() = 0;
	virtual void PropertyChanged(uint16_t propertyId, PropertyChangedBy changedBy) = 0;
};

#endif