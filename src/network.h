#ifndef network_h
#define network_h

#include <Arduino.h>
#include "DeviceConfig.h"

// Initializes DNS responder, called automatically from initNetwork()
bool initNetBIOS(const String& hostname);
// Initializes device as station
bool initNetwork(const DeviceConfig& config);
// Initiates connection procedure
bool beginConnect(const String& ssid, const String& password);
// Can be called from redefied onConnectionUpdated(bool connected)
void logConnectionStatus(bool connected);
// Must be called every loop iteration, returns true if connected, otherwise false
bool watchConnection();

#endif