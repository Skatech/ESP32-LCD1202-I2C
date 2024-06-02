#ifndef DeviceConfig_h
#define DeviceConfig_h

#include <Arduino.h>
#include <IPAddress.h>

class DeviceConfig {
public:
    String ssid;
    String password;
    String hostname;

    IPAddress address;
    IPAddress gateway;
    IPAddress subnet;
    IPAddress dns;

    bool load();
    bool save();
    bool exec(const String& command);
};

#endif
