#include "EscapeAutomate.hpp"


// **************************************************************
// Example puzzle
// **************************************************************

class ExamplePuzzleClass : public Puzzle
{
public:
	ExamplePuzzleClass(uint16_t puzzleId, String name, Difficulty difficulty = Difficulty_Normal) : Puzzle(puzzleId, name, difficulty)
	{
	}

	BoolProperty*	testBool;
	NumberProperty* testInt;
	StringProperty* testString;
	ListProperty*	testList;

	void Setup()
	{
		testBool = new BoolProperty(1, "testBool", false);
		testInt = new NumberProperty(2, "testNumber", 27);
		testString = new StringProperty(3, "testString", "blablabla");
		testList = new ListProperty(4, "testList", false, true, new String[2]{ "value1", "value2" }, 2, new String[2]{ "value1","value2" }, 0);

		RegisterProperty(testBool);
		RegisterProperty(testInt);
		RegisterProperty(testString);
		RegisterProperty(testList);

		testBool->ChangeProperty(true);
	}

	void Start()
	{
	}

	void Stop()
	{
	}

	void Reset()
	{
	}

	void Loop()
	{
	}

	void Notification(uint16_t senderId, const char* jsonValue)
	{
	}

	void Completed()
	{
	}

	void PropertyChanged(uint16_t propertyId, PropertyChangedBy changedBy)
	{
		switch (propertyId)
		{
		case 1: // testBool
			break;
		case 2: // testNumber
			break;
		case 3: // testString
			break;
		case 4: // testList
			break;
		}
	}
};
ExamplePuzzleClass ExamplePuzzle(0, "ExamplePuzzle");


void setup() {
	Serial.begin(115200);
	delay(500);

	EscapeAutomate.RegisterPuzzle(&ExamplePuzzle);

	EscapeAutomate.Setup("projectId", "Basic example", "wifiSsid", "wifiPassword", "masterPassword");
}

void loop() {
	EscapeAutomate.Loop();
}