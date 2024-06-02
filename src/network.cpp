#include <WiFi.h>
#include <ESPmDNS.h>
#include "network.h"

bool initNetBIOS(const String& hostname) {
    if(hostname.length()) {
        return MDNS.begin(hostname.c_str()); // MDNS responder started
    }
    MDNS.end();
    return true;
}

bool initNetwork(const DeviceConfig& config) {
    return (!WiFi.isConnected() || WiFi.disconnect()) && WiFi.mode(WIFI_STA)
        && WiFi.config(config.address, config.gateway, config.subnet, config.dns)
        && WiFi.hostname(config.hostname) && initNetBIOS(config.hostname);
}

bool beginConnect(const String& ssid, const String& password) {
    if (WiFi.disconnect() && ssid.length()) {
        const auto status = WiFi.begin(ssid, password);
        return status == WL_DISCONNECTED || status == WL_CONNECTED;
    }
    return false;
}

void logConnectionStatus(bool connected) {
    if (connected) {
        Serial.print("Connection estabilished, address: ");
        Serial.println(WiFi.localIP());
    }
    else Serial.println("Connection LOST");
}

void __attribute__((weak)) onConnectionUpdated(bool connected) { // weak, can be redefined
    logConnectionStatus(connected);
}

bool watchConnection() {
    static bool connected = false;
    const auto status = WiFi.status();
    if (connected) {
        if (status != WL_CONNECTED) {
            digitalWrite(LED_BUILTIN, HIGH); // ESP8266 invert(LOW)
            onConnectionUpdated(connected = false);
        }
    }
    else {
        if (status == WL_CONNECTED) {
            digitalWrite(LED_BUILTIN, LOW);
            onConnectionUpdated(connected = true);
        }
    }
    return connected;
}
