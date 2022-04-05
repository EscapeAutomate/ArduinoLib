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
		switch (PuzzleObject->Status)
		{
		case PuzzleStatus_Stopped:
		case PuzzleStatus_Reseted:
		case PuzzleStatus_Completed:
			break;
		case PuzzleStatus_Started:
		case PuzzleStatus_Disconnected:
		case PuzzleStatus_Connected:
		default:
			break;
		}
	}

	void Notification(uint16_t senderId, const char* jsonValue)
	{
		DynamicJsonDocument jsonDoc(800);

		DeserializationError error = deserializeJson(jsonDoc, jsonValue);

		if (error) {
			Serial.print("deserializeJson() failed: ");
			Serial.println(error.c_str());
			return;
		}

		if (senderId == 12)
		{
			bool MyObject_prop1 = jsonDoc["MyObject"]["prop1"];
		}
	}

	void Completed()
	{
		UpdatePuzzleStatus(PuzzleStatus_Completed);
	}

	void PropertyChanged(uint16_t propertyId, PropertyChangedBy changedBy)
	{
		switch (propertyId)
		{
		case 1: // testBool
		{
			DynamicJsonDocument jsonDoc(800);
			JsonObject hub = jsonDoc.createNestedObject("MyObject");

			hub["prop1"] = true;

			String output;
			serializeJson(jsonDoc, output);

			EscapeAutomate.SendNotificationToPuzzle(this->PuzzleObject->PuzzleId, 12, output.c_str());
			break;
		}
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