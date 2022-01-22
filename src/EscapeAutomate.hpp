#ifndef _EscapeAutomate_h
#define _EscapeAutomate_h
#pragma once

// Change _ES_LOGLEVEL_ to set tracing and logging verbosity
// 0: DISABLED: no logging
// 1: ERROR: errors
// 2: WARN: errors and warnings
// 3: INFO: errors, warnings and informational (default)
// 4: DEBUG: errors, warnings, informational and debug

#define _ES_LOG_DISABLED	0
#define _ES_LOG_ERROR		1
#define _ES_LOG_WARN		2
#define _ES_LOG_INFO		3
#define _ES_LOG_DEBUG		4

#ifndef _ES_LOGLEVEL_
#define _ES_LOGLEVEL_       _ES_LOG_INFO
#endif

#include "EscLogs.h"
#include "Puzzle.h"
#include "Objects/HubProperty.h"

#define MASTER_HTTP_PROJECT String("/api/Project/")
#define MASTER_HTTP_HUB String("/api/Hub/")
#define MASTER_HTTP_PUZZLE String("/api/Puzzle/")

class EscapeAutomateClass
{
private:
	const char* ssid;
	const char* wifiPassword;
	const char* masterPassword;
	bool masterFound = false;
	String masterIp;
	uint16_t masterPort;
	unsigned long lastSendedMessageTime = 0;

	bool CallRegisterHub();
	bool CallPing();
	bool IsProjectLoadedOnMaster();
	bool SendMessage(MessageId mId, String message);

public:
	std::map<uint16_t, Puzzle*> CustomPuzzles;
	uint8_t NumberOfPuzzle = 0;
	HubProperty Hub;

	void Setup(const char* projectId, const char* hubName, const char* ssid, const char* wifiPassword, const char* masterPassword);
	void RegisterPuzzle(Puzzle* puzzle);
	void Loop();
	void UpdateEngineStatus(HubConnectionStatus status);
	bool SendHubPropertyChanged(uint16_t propertyId, String propertyName, String value);
	bool SendPuzzlePropertyChanged(uint16_t puzzleId, uint16_t propertyId, String propertyName, String value, PropertyChangedBy propertyChangedBy);
	void ManagePuzzleStatusChange(uint16_t puzzleId, PuzzleStatus puzzleStatus);
	void ChangeProp(uint16_t PuzzleId, uint16_t PropertyId, const char* propertyName, const char* jsonData);
	bool SendNotificationToPuzzle(uint16_t senderPuzzleId, uint16_t destPuzzleId, const char* Notification);
	void Notification(uint16_t senderPuzzleId, uint16_t puzzleId, const char* jsonValue);
	void UpdateStatusLed(StatusLedColors color, bool isError);
	//void SendNotificationToMaster(Puzzle* puzzle, const char* message);
	//void SendPropertyChanged(Puzzle* puzzle, Property* property);
	//void SendUpdatePuzzleProperty(Puzzle* puzzle, const char* index, bool boolValue, int intValue, char* strValue);
};

extern EscapeAutomateClass EscapeAutomate;

#endif

