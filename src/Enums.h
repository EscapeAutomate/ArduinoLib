#pragma once

enum HubConnectionStatus
{
	HubConnectionStatus_NotConnected = 0,
	HubConnectionStatus_Connected = 1
};

enum WellKnowObjectIds
{
	WellKnowObjectIds_Hub = 1,
	WellKnowObjectIds_Puzzle = 2,
	WellKnowObjectIds_Bool = 3,
	WellKnowObjectIds_Int = 4,
	WellKnowObjectIds_String = 5,
	WellKnowObjectIds_List = 6
};

enum PuzzleStatus
{
	PuzzleStatus_Started = 0,
	PuzzleStatus_Stopped = 1,
	PuzzleStatus_Reseted = 2,
	PuzzleStatus_Completed = 3,
	PuzzleStatus_Disconnected = 4,
	PuzzleStatus_Connected = 5
};

enum Difficulty
{
	Difficulty_Easy = 0,
	Difficulty_Normal = 1,
	Difficulty_Hard = 2
};

enum MessageId
{
	MessageId_Register = 1,
	MessageId_Ping = 2,
	MessageId_HubPropertyChanged = 3,
	MessageId_PuzzlePropertyChanged = 4,
	MessageId_Notification = 5,
	MessageId_HubChangeProperty = 6,
	MessageId_PuzzleChangeProperty = 7,
	MessageId_Error = 8,
	MessageId_ScenarioStatusChanged = 9
};

enum StatusLedColors
{
	StatusLedColors_NotConnected = 0,
	StatusLedColors_ConnectedToWifi = 1,
	StatusLedColors_ConnectedToMaster = 2
};

enum PropertyChangedBy
{
	PropertyChangedBy_MyCode = 0,
	PropertyChangedBy_Master = 1
};