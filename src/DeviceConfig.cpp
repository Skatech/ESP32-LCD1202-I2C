#include <ConfigUtils.h>
#include "DeviceConfig.h"

// #define CONFIGURATION_FILE_PATH "/config/device-config.json"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winvalid-offsetof" // disable IPAddress fields layout warning
const char* __DeviceConfigFilePath = "/config/device-config.json";
const FieldHandle __DeviceConfigFields[] = {
    { "ssid", FieldType::StringT, offsetof(DeviceConfig, ssid) },
    { "password", FieldType::StringT, offsetof(DeviceConfig, password) },
    { "hostname", FieldType::StringT, offsetof(DeviceConfig, hostname) },

    { "address", FieldType::IPAddressT, offsetof(DeviceConfig, address) },
    { "gateway", FieldType::IPAddressT, offsetof(DeviceConfig, gateway) },
    { "subnet", FieldType::IPAddressT, offsetof(DeviceConfig, subnet) },
    { "dns", FieldType::IPAddressT, offsetof(DeviceConfig, dns) },
    //{ "webport", FieldType::IntT, offsetof(DeviceConfig, webport) }
};
#pragma GCC diagnostic pop

bool DeviceConfig::load() {
    // ssid = password = hostname = "";
    return ConfigUtils::loadFromJSON(__DeviceConfigFields,
        sizeof(__DeviceConfigFields) / sizeof(FieldHandle), this, __DeviceConfigFilePath);
}

bool DeviceConfig::save() {
    return ConfigUtils::saveToJSON(__DeviceConfigFields,
        sizeof(__DeviceConfigFields) / sizeof(FieldHandle), this, __DeviceConfigFilePath);
}

bool DeviceConfig::exec(const String& command) {
    if (command == "show-config") {
        ConfigUtils::printAllFields(__DeviceConfigFields,
            sizeof(__DeviceConfigFields) / sizeof(FieldHandle), this);
    }
    else if (command == "save-config") {
        Serial.print("Configuration writing... ");
        Serial.println(save() ? "SUCCEEDED" : "FAILED");
    }
    else return ConfigUtils::printOrUpdateField(__DeviceConfigFields,
        sizeof(__DeviceConfigFields) / sizeof(FieldHandle), this, command);
    return true;
}
