#include "EscapeAutomate.hpp"
#include <WebSocket/ArduinoWebsockets.h>
#include <WebSocket/tiny_websockets/internals/wscrypto/crypto.hpp>
#include <ESPmDNS.h>
#include <ArduinoJson.h>
#include <ArduinoOTA.h>
#include "WiFi.h"
#include "defines.h"
#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel* EscapeAutomatePixels;

using namespace websockets;

void onMessageCallback(WebsocketsMessage message);
void onEventsCallback(WebsocketsEvent event, String data);

EscapeAutomateClass EscapeAutomate;
WebsocketsClient wsClient;

void onMessageCallback(WebsocketsMessage message)
{
	ESC_LOGINFO1("recv message:", message.data());

	DynamicJsonDocument doc(4096);

	DeserializationError error = deserializeJson(doc, message.data());

	if (error) {
		ESC_LOGERROR1("deserializeJson() failed: ", error.f_str());
		return;
	}

	const char* m = doc["message"];
	MessageId type = doc["type"];

	DynamicJsonDocument doc2(4096);

	error = deserializeJson(doc2, m);

	if (error) {
		ESC_LOGERROR1("deserializeJson() failed: ", error.f_str());
		return;
	}

	uint16_t puzzleId;

	switch (type)
	{
	case MessageId_PuzzleChangeProperty:
		puzzleId = doc2["puzzleId"]; // 10

		for (JsonObject elem : doc2["changeProperties"].as<JsonArray>())
		{
			int propertyId = elem["propertyId"]; // 1, 2

			for (JsonObject elem : elem["values"].as<JsonArray>())
			{
				const char* name = elem["name"]; // "test", "test3"
				const char* jsonValue = elem["jsonValue"]; // "test2", "test4"

				EscapeAutomate.ChangeProp(puzzleId, propertyId, name, jsonValue);
			}
		}
		break;

	case MessageId_Notification:
		uint16_t senderPuzzleId2 = doc2["senderPuzzleId"]; // 10
		puzzleId = doc2["destinationPuzzleId"]; // 10
		const char* jsonValue2 = doc2["jsonValue"]; // 10

		EscapeAutomate.Notification(senderPuzzleId2, puzzleId, jsonValue2);

		break;
	}
}

void onEventsCallback(WebsocketsEvent event, String data)
{
	if (event == WebsocketsEvent::ConnectionOpened)
	{
		ESC_LOGINFO("Connnection Opened");
	}
	else if (event == WebsocketsEvent::ConnectionClosed)
	{
		ESC_LOGINFO("Wifi not connected...");
	}
	else if (event == WebsocketsEvent::GotPing)
	{
		ESC_LOGINFO("Got a Ping!");
	}
	else if (event == WebsocketsEvent::GotPong)
	{
		ESC_LOGINFO("Got a Pong!");
	}
}

bool IsSuccessCode(int httpResponseCode)
{
	if (httpResponseCode >= 200 && httpResponseCode < 300) {
		ESC_LOGINFO1("HTTP Ok code:", httpResponseCode);
		return true;
	}
	else {
		ESC_LOGERROR1("HTTP Error code:", httpResponseCode);
		return false;
	}
}

void EscapeAutomateClass::ChangeProp(uint16_t puzzleId, uint16_t propertyId, const char* propertyName, const char* jsonData)
{
	if (puzzleId == 65535) // all puzzles
	{
		for (auto const& puzzle : CustomPuzzles)
		{
			for (auto const& prop : puzzle.second->Properties)
			{
				if (prop.second->PropertyId == propertyId)
				{
					prop.second->ChangeProperty(propertyName, jsonData, PropertyChangedBy_Master);
					break;
				}
			}
		}
	}
	else if (CustomPuzzles.count(puzzleId) == 1)
	{
		for (auto const& prop : CustomPuzzles[puzzleId]->Properties)
		{
			if (prop.second->PropertyId == propertyId)
			{
				prop.second->ChangeProperty(propertyName, jsonData, PropertyChangedBy_Master);
				break;
			}
		}
	}
}

bool EscapeAutomateClass::SendNotificationToPuzzle(uint16_t senderPuzzleId, uint16_t destPuzzleId, const char* Notification)
{
	DynamicJsonDocument doc(1024);
	String output;

	doc["senderPuzzleId"] = senderPuzzleId;
	doc["destinationPuzzleId"] = destPuzzleId;
	doc["jsonValue"] = Notification;

	serializeJson(doc, output);

	return SendMessage(MessageId_Notification, output);
}

void EscapeAutomateClass::Setup(const char* projectId, const char* hubName, const char* wifiSsid, const char* wifiPassword, const char* masterPassword, const char* server_mdns_address = "escapeautomatemaster2", bool useNeopixel = true)
{
	this->mdnsAddress = server_mdns_address;
	this->useNeopixel = useNeopixel;
	Hub.ProjectId = projectId;
	Hub.Name = hubName;

	this->ssid = wifiSsid;
	this->wifiPassword = wifiPassword;
	this->masterPassword = masterPassword;

	EscapeAutomatePixels = new Adafruit_NeoPixel(1, 18, NEO_GRB + NEO_KHZ800);

	uint64_t chipid = ESP.getEfuseMac();
	char macAddress[13];
	memset(macAddress, '\0', 13);
	sprintf(macAddress, "%X%X", (int)(chipid >> 32), (int)(chipid));

	ESC_LOGINFO1("mac address:", macAddress);

	Hub.Status = HubConnectionStatus_NotConnected;
	Hub.Mac = macAddress;

	wsClient.setInsecure();

	// run callback when messages are received
	wsClient.onMessage(onMessageCallback);

	// run callback when events are occuring
	wsClient.onEvent(onEventsCallback);

	//wsClient.setAuthorization((Hub.ProjectId + ";" + Hub.Mac).c_str(), masterPassword);
	String auth = Hub.ProjectId + ";" + Hub.Mac;
	auth += ":";
	auth += masterPassword;

	WSString b64Auth = crypto::base64Encode((uint8_t*)auth.c_str(), auth.length());
	WSString basic = "Basic ";

	wsClient.addHeader("Authorization", basic.append(b64Auth).c_str());

	UpdateStatusLed(StatusLedColors_NotConnected, false);

	ArduinoOTA.setHostname(Hub.Name.c_str());
	ArduinoOTA.setPassword(this->masterPassword);
	ArduinoOTA
		.onStart([]() {
		String type;
		if (ArduinoOTA.getCommand() == U_FLASH)
			type = "sketch";
		else // U_SPIFFS
			type = "filesystem";

		// NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
		Serial.println("Start updating " + type);
			})
		.onEnd([]() {
				Serial.println("\nEnd");
			})
				.onProgress([](unsigned int progress, unsigned int total) {
				Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
					})
				.onError([](ota_error_t error) {
						Serial.printf("Error[%u]: ", error);
						if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
						else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
						else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
						else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
						else if (error == OTA_END_ERROR) Serial.println("End Failed");
					});

					for (auto const& puzzle : CustomPuzzles)
					{
						puzzle.second->Setup();

						xTaskCreate(
							puzzle.second->InnerLoop,    // Function that should be called
							puzzle.second->PuzzleObject->Name.c_str(),   // Name of the task (for debugging)
							8000,            // Stack size (bytes)
							puzzle.second,            // Parameter to pass
							1,               // Task priority
							NULL             // Task handle
						);
					}

					ESC_LOGINFO("setup done");
}

void EscapeAutomateClass::RegisterPuzzle(Puzzle* puzzle)
{
	if (CustomPuzzles.size() < 3)
	{
		CustomPuzzles.insert(std::pair<uint16_t, Puzzle*>(puzzle->PuzzleObject->PuzzleId, puzzle));
	}
	else
	{
		ESC_LOGERROR("Too many puzzle registered (max 4)!");
		UpdateStatusLed(StatusLedColors_NotConnected, true);
	}
}

void EscapeAutomateClass::Loop()
{
	wsClient.poll();
	ArduinoOTA.handle();

	if (WiFi.status() != WL_CONNECTED)
	{
		UpdateStatusLed(StatusLedColors_NotConnected, false);

		ESC_LOGINFO("Wifi not connected...");
		WiFi.begin(ssid, wifiPassword);
		ESC_LOGINFO("Begin WIFI connection...");

		while (WiFi.status() != WL_CONNECTED) {
			delay(500);
			Serial.print(".");
		}

		ESC_LOGINFO3("Wifi connected to", WiFi.SSID(), "IP address:", WiFi.localIP());

		/*if (!MDNS.begin(Hub.Mac.c_str())) {
			ESC_LOGERROR("Error setting up MDNS responder!");

			UpdateStatusLed(StatusLedColors_NotConnected, true);
		}*/

		// ArduinoOTA already register MDNS
		ArduinoOTA.begin();

		UpdateStatusLed(StatusLedColors_ConnectedToWifi, false);
	}
	else // Connected to wifi
	{
		if (Hub.Status == HubConnectionStatus_NotConnected)
		{
			int n = MDNS.queryService(mdnsAddress, "tcp");

			if (n == 0)
			{
				ESC_LOGINFO("no master found");
			}
			else
			{
				if (_ES_LOGLEVEL_ >= _ES_LOG_INFO)
				{
					Serial.print(n);
					Serial.println(" master(s) found");

					for (int i = 0; i < n; ++i)
					{
						// Print details for each service found
						Serial.print(i + 1);
						Serial.print(": ");
						Serial.print(MDNS.hostname(i));
						Serial.print(" (");
						Serial.print(MDNS.IP(i));
						Serial.print(":");
						Serial.print(MDNS.port(i));
						Serial.println(")");
					}
				}

				for (int i = 0; i < n; ++i)
				{
					this->masterIp = MDNS.IP(i).toString();
					this->masterPort = MDNS.port(i);

					if (IsProjectLoadedOnMaster())
					{
						// Connect to server
						bool connected = wsClient.connect("wss://" + this->masterIp + ":" + this->masterPort + "/hubmanager");

						if (connected)
						{
							if (CallRegisterHub())
							{
								masterFound = true;
								ESC_LOGINFO("master connected !");

								UpdateEngineStatus(HubConnectionStatus_Connected);
								UpdateStatusLed(StatusLedColors_ConnectedToMaster, false);
								return;
							}
							else
							{
								ESC_LOGERROR("Cannot register on the master !");
							}
						}
						else
						{
							ESC_LOGERROR("Websocket connection error !");
						}
					}
				}
			}
		}
		else // Master found and connected
		{
			if (millis() - lastSendedMessageTime > 4000)
			{
				if (!CallPing())
				{
					// TODO close the websocket
					UpdateEngineStatus(HubConnectionStatus_NotConnected);
					UpdateStatusLed(StatusLedColors_ConnectedToWifi, false);
				}
			}
		}
	}
}

void EscapeAutomateClass::UpdateEngineStatus(HubConnectionStatus status)
{
	Hub.Status = status;

	SendHubPropertyChanged(0, "Status", String(Hub.Status));

	for (auto const& puzzle : CustomPuzzles)
	{
		if (status == HubConnectionStatus_Connected)
		{
			puzzle.second->UpdatePuzzleStatus(PuzzleStatus_Connected);
		}
		else
		{
			puzzle.second->UpdatePuzzleStatus(PuzzleStatus_Disconnected);
		}
	}
}

bool EscapeAutomateClass::SendHubPropertyChanged(uint16_t propertyId, String propertyName, String value)
{
	DynamicJsonDocument doc(1024);
	String output;

	JsonArray propertiesChanged = doc.createNestedArray("propertiesChanged");

	JsonObject propertiesChanged_0 = propertiesChanged.createNestedObject();
	propertiesChanged_0["propertyId"] = propertyId;

	JsonObject propertiesChanged_0_values_0 = propertiesChanged_0["values"].createNestedObject();
	propertiesChanged_0_values_0["name"] = propertyName;
	propertiesChanged_0_values_0["jsonValue"] = value;

	serializeJson(doc, output);

	//ESC_LOGINFO1("HubPropertyChanged: ", output);

	return SendMessage(MessageId_HubPropertyChanged, output);
}

bool EscapeAutomateClass::SendPuzzlePropertyChanged(uint16_t puzzleId, uint16_t propertyId, String propertyName, String value, PropertyChangedBy propertyChangedBy)
{
	CustomPuzzles[puzzleId]->PropertyChanged(propertyId, propertyChangedBy);
	DynamicJsonDocument doc(1024);
	String output;

	doc["puzzleId"] = puzzleId;
	JsonArray propertiesChanged = doc.createNestedArray("propertiesChanged");

	JsonObject propertiesChanged_0 = propertiesChanged.createNestedObject();
	propertiesChanged_0["propertyId"] = propertyId;

	JsonObject propertiesChanged_0_values_0 = propertiesChanged_0["values"].createNestedObject();
	propertiesChanged_0_values_0["name"] = propertyName;
	propertiesChanged_0_values_0["jsonValue"] = value;

	serializeJson(doc, output);

	//ESC_LOGINFO1("PuzzlePropertyChanged: ", output);

	return SendMessage(MessageId_PuzzlePropertyChanged, output);
}

bool EscapeAutomateClass::CallRegisterHub()
{
	DynamicJsonDocument doc(4000);

	JsonObject hub = doc.createNestedObject("hub");
	Hub.FillJson(&hub);

	/*Pas pour tout de suite
	JsonArray exposedProperties = doc.createNestedArray("exposedProperties");
	exposedProperties.add("{\r\n  \"propertyType\": 4,\r\n  \"value\": 20,\r\n  \"name\": \"intProperty\",\r\n  \"id\": 0\r\n}");
	exposedProperties.add("{\r\n  \"propertyType\": 5,\r\n  \"value\": \"Lorem ipsum\",\r\n  \"name\": \"stringProperty\",\r\n  \"id\": 0\r\n}");
	exposedProperties.add("{\r\n  \"propertyType\": 3,\r\n  \"value\": true,\r\n  \"name\": \"boolProperty\",\r\n  \"id\": 0\r\n}");*/

	JsonArray puzzles = doc.createNestedArray("puzzles");

	for (auto const& puz : CustomPuzzles)
	{
		JsonObject puzzle = puzzles.createNestedObject();

		JsonObject puzzleObj = puzzle.createNestedObject("puzzleObj");
		puz.second->PuzzleObject->FillJson(&puzzleObj);

		JsonArray puzzle_exposedProperties = puzzle.createNestedArray("exposedProperties");

		//custom property
		for (std::pair<const uint16_t, BaseProperty*> prop : puz.second->Properties)
		{
			puzzle_exposedProperties.add(prop.second->Serialize());
		}
	}

	String output;
	serializeJson(doc, output);

	if (_ES_LOGLEVEL_ > 2)
	{
		Serial.print("Register: ");
		serializeJsonPretty(doc, Serial);
		Serial.println();
		Serial.println();
	}

	return SendMessage(MessageId_Register, output);

	/*httpClient.begin(*client, masterIp, masterPort, MASTER_HTTP_HUB + "Register", true);
	httpClient.addHeader("Content-Type", "application/json");

	int httpResponseCode = httpClient.POST(output);

	if (IsSuccessCode(httpResponseCode)) {
#if DEBUG_MSG
		Serial.println("Registered on master");
#endif
		lastSendedMessageTime = millis();

		return true;
	}
	else
	{
		Serial.println("ERROR:" + httpClient.getString());
	}

	return false;*/
}

bool EscapeAutomateClass::CallPing()
{
	DynamicJsonDocument doc(100);
	String output;

	doc["mac"] = Hub.Mac;

	serializeJson(doc, output);

	return SendMessage(MessageId_Ping, output);

	/*httpClient.begin(*client, masterIp, masterPort, MASTER_HTTP_HUB + "Ping", true);
	httpClient.addHeader("Content-Type", "application/json");

	int httpResponseCode = httpClient.POST(output);

	if (IsSuccessCode(httpResponseCode)) {
#if DEBUG_MSG
		Serial.println("Ping OK");
#endif
		lastSendedMessageTime = millis();

		return true;
	}

	return false;*/
}

bool EscapeAutomateClass::IsProjectLoadedOnMaster()
{
	WiFiClientSecure* client = new WiFiClientSecure();
	bool res = false;

	if (client)
	{
		client->setInsecure();

		{
			HTTPClient http;
			http.begin(*client, "https://" + masterIp + ":" + masterPort + MASTER_HTTP_PROJECT + "LoadedProjects");

			int httpResponseCode = http.GET();

			if (IsSuccessCode(httpResponseCode)) {
				DynamicJsonDocument filter(100);
				filter[0]["projectId"] = true;

				DynamicJsonDocument doc(2048);
				deserializeJson(doc, http.getString(), DeserializationOption::Filter(filter));

				if (_ES_LOGLEVEL_ > 2)
				{
					Serial.print("ProjectLoaded response: ");
					serializeJsonPretty(doc, Serial);
					Serial.println();
				}

				for (JsonObject elem : doc.as<JsonArray>()) {
					if (strcmp(elem["projectId"], Hub.ProjectId.c_str()) == 0)
					{
						http.end();
						ESC_LOGINFO("Project loaded on master");
						res = true;
						break;
					}
				}
				http.end();

				if (!res)
				{
					ESC_LOGINFO("Project NOT loaded on master");
				}
			}
		}
	}

	delete client;

	return res;
}

bool EscapeAutomateClass::SendMessage(MessageId mId, String message)
{
	if (mId != MessageId_Register && Hub.Status == HubConnectionStatus_NotConnected)
	{
		return false;
	}

	if (message.length() > 4000)
	{
		ESC_LOGWARN1("Message bigger than 4000 bytes: ", message.length());
		ESC_LOGWARN(message);
	}

	lastSendedMessageTime = millis();

	DynamicJsonDocument doc(4096);
	String output;

	doc["type"] = mId;
	doc["message"] = message;

	serializeJson(doc, output);

	ESC_LOGINFO1("Message size:", measureJson(doc));
	ESC_LOGINFO2("Send message:", mId, message);

	return wsClient.send(output);
}

void EscapeAutomateClass::UpdateStatusLed(StatusLedColors status, bool isError)
{
	if (useNeopixel)
	{
		EscapeAutomatePixels->setBrightness(10);
		EscapeAutomatePixels->begin();

		if (isError)
		{
			while (true)
			{
				EscapeAutomatePixels->setPixelColor(0, EscapeAutomatePixels->Color(255, 0, 0));
				EscapeAutomatePixels->show();
				delay(500);
				EscapeAutomatePixels->setPixelColor(0, EscapeAutomatePixels->Color(0, 0, 0));
				EscapeAutomatePixels->show();
				delay(500);
			}
		}

		switch (status)
		{
		case StatusLedColors_NotConnected:
			EscapeAutomatePixels->setPixelColor(0, EscapeAutomatePixels->Color(255, 0, 0));
			break;
		case StatusLedColors_ConnectedToWifi:
			EscapeAutomatePixels->setPixelColor(0, EscapeAutomatePixels->Color(0, 0, 255));
			break;
		case StatusLedColors_ConnectedToMaster:
			EscapeAutomatePixels->setPixelColor(0, EscapeAutomatePixels->Color(0, 255, 0));
			break;
		default:
			break;
		}

		EscapeAutomatePixels->show();
	}
}

void EscapeAutomateClass::Notification(uint16_t senderPuzzleId, uint16_t puzzleId, const char* jsonValue)
{
	if (CustomPuzzles.count(puzzleId) == 1)
	{
		CustomPuzzles[puzzleId]->Notification(senderPuzzleId, jsonValue);
	}
}

void EscapeAutomateClass::ManagePuzzleStatusChange(uint16_t puzzleId, PuzzleStatus puzzleStatus)
{
	if (CustomPuzzles.count(puzzleId) == 1)
	{
		switch (puzzleStatus)
		{
		case PuzzleStatus_Started:
			CustomPuzzles[puzzleId]->Start();
			break;
		case PuzzleStatus_Stopped:
			CustomPuzzles[puzzleId]->Stop();
			break;
		case PuzzleStatus_Reseted:
			CustomPuzzles[puzzleId]->Reset();
			break;
		case PuzzleStatus_Completed:
			CustomPuzzles[puzzleId]->Completed();
			break;
		}
	}
}
