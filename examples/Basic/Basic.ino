#include <Adafruit_MCP23X17.h>
#include "EscapeAutomate2.hpp"


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

	Adafruit_MCP23X17 mcp;

	void Setup()
	{
		testBool = new BoolProperty(1, "testBool");
		testInt = new NumberProperty(2, "testNumber");
		testString = new StringProperty(3, "testString");
		testList = new ListProperty(4, "testList", false, true, new String[2]{ "value1", "value2" }, 2, new String[2]{ "value1","value2" }, 0);

		RegisterProperty(testBool);
		RegisterProperty(testInt);
		RegisterProperty(testString);
		RegisterProperty(testList);

		// uncomment appropriate mcp.begin
		if (!mcp.begin_I2C(0x21)) {
			//if (!mcp.begin_SPI(CS_PIN)) {
			Serial.println("Error.");
			while (1);
		}

		mcp.pinMode(0, OUTPUT);
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

	void PropertyChanged(uint16_t propertyId)
	{
		switch (propertyId)
		{
		case 1: // testBool
			mcp.digitalWrite(0, testBool->Value);
			break;
		case 2: // testNumber
			break;
		case 3: // testString
			break;
		}
	}
};
ExamplePuzzleClass ExamplePuzzle(0, "ExamplePuzzle");


void setup() {
	Wire.begin(6, 7);
	Serial.begin(115200);
	delay(500);

	EscapeAutomate.RegisterPuzzle(&ExamplePuzzle);

	EscapeAutomate.Setup("9FEB3BB60F834F708E79D7FA6C78EFC3", "Basic example", "Echappatoire", "eX66rFfj5qAFymxa", "VFTEQjfa3anBUEs6zQAI");
	//EscapeAutomate.Setup("9FEB3BB60F834F708E79D7FA6C78EFC3", "Basic example", "OrneTHD-6996", "4A780423489CBB91", "aYz4qBVfwlb4JKPT4CYi");
}

void loop() {
	EscapeAutomate.Loop();
}